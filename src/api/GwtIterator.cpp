//
// Created by AMakarenya on 01.04.2016.
//

#include "GwtIterator.h"
#include "GwtObject.h"
#include "GwtType.h"
#include "GwtValue.h"
#include "GwtPathExpression.h"
#include <boost/lexical_cast.hpp>


namespace vantagefx {
    namespace api {

		GwtComplexIterator::GwtComplexIterator(const GwtConstObjectPtr &object,
			InternalIterator it, InternalIterator end)
			: _it(it),
			  _end(end),
			  _object(object)
	    {}

		void GwtComplexIterator::advance()
		{
			++_it;
		}

		bool GwtComplexIterator::empty() const
		{
			return _it == _end;
		}

		GwtValue GwtComplexIterator::get() const
		{
			return _object->value((*_it)->name());
		}

		GwtFieldPtr GwtComplexIterator::field() const
		{
			return *_it;
		}

		std::string GwtComplexIterator::part() const
		{
			return (*_it)->name();
		}

	    GwtMapIterator::GwtMapIterator(InternalIterator it, InternalIterator end)
			: _field(fptr("value")),
			  _it(it),
			  _end(end)
		{ }

		void GwtMapIterator::advance()
		{
			++_it;
		}

		bool GwtMapIterator::empty() const
		{
			return _it == _end;
		}

		GwtValue GwtMapIterator::get() const
		{
			return _it->second;
		}

		GwtFieldPtr GwtMapIterator::field() const
		{
			return _field;
		}

		std::string GwtMapIterator::part() const
		{
			return _it->first;
		}

	    GwtArrayIterator::GwtArrayIterator(const GwtConstObjectPtr &object)
			  : _object(object),
			    _field(fptr("value")),
			    _index(0),
			    _length(object->value("length").intValue())
	    { }

		void GwtArrayIterator::advance()
		{
			_index++;
		}

		bool GwtArrayIterator::empty() const
		{
			return _index == _length;
		}

		GwtValue GwtArrayIterator::get() const
		{
			return _object->value(boost::lexical_cast<std::string>(_index));
		}

		GwtFieldPtr GwtArrayIterator::field() const
		{
			return _field;
		}

		std::string GwtArrayIterator::part() const
		{
			auto index = boost::lexical_cast<std::string>(_index);
			return index;
		}

	    std::string GwtArrayIterator::name() const
		{
			auto index = boost::lexical_cast<std::string>(_index);
			auto value = _object->value(index);
			auto obj = value.toObject();
			if (obj) {
				auto primary = obj->type()->primary();
				if (!primary.empty()) {
					auto name = obj->value(primary).valueString();
					return "[" + primary + "=" + name + "]";
				}
			}
			return index;
		}

	    GwtQueryIterator::GwtQueryIterator(const GwtPath &path, const GwtValue &value,
			const std::vector<GwtValue> &values, const std::string &prefix)
			: _path(path),
			  _values(values)
	    {
		    load(_path.begin(), value, "", prefix, prefix);
		}

	    GwtQueryIterator::GwtQueryIterator(GwtQueryIterator &&rhs)
			: _path(std::move(rhs._path)),
			  _stack(std::move(rhs._stack)),
			  _current(std::move(rhs._current))
		{ }

	    GwtQueryIterator &GwtQueryIterator::operator=(GwtQueryIterator &&rhs)
	    {
			if (this == &rhs) return *this;
			_path = std::move(rhs._path);
			_stack = std::move(rhs._stack);
			_current = std::move(rhs._current);
			return *this;
		}

	    bool GwtQueryIterator::set(const GwtValue &value, const std::string &path)
		{
			_current.value = value;
			_current.path = path;
			return true;
		}

	    GwtQueryIterator::GwtQueryIterator()
        { }

        bool GwtQueryIterator::load(GwtPath::iterator it, const GwtValue &item, std::string path, const std::string &part, const std::string &name)
        {
			if (!path.empty()) path += "/" + name;
			else path = name;
            if (it == _path.end()) {
				return set(item, path);
            }
			if (it->name() == "int()") {
				try {
					return set(GwtValue(boost::lexical_cast<int>(part)), path);
				}
				catch(boost::bad_lexical_cast) {
					return set(GwtValue(), path);
				}
			}
			if (it->name() == "long()") {
				try {
					return set(GwtValue(boost::lexical_cast<int64_t>(part)), path);
				} 
				catch (boost::bad_lexical_cast) {
					return set(GwtValue(), path);
				}
			}
			if (it->name() == "double()") {
				try {
					return set(GwtValue(boost::lexical_cast<double>(part)), path);
				}
				catch (boost::bad_lexical_cast) {
					return set(GwtValue(), path);
				}
			}
			if (it->name() == "key()") {
				return set(GwtValue(part), path);
			}
			if (it->name() == "to_string()") {
				return set(GwtValue(item.toString()), path);
			}
			if (it->name() == "to_int()") {
				return set(GwtValue(item.toInt()), path);
			}
			if (it->name() == "to_double()") {
				return set(GwtValue(item.toDouble()), path);
			}
			if (it->name() == "to_long()") {
				return set(GwtValue(item.toLong()), path);
			}

			auto obj = item.toObject();
			if (!it->name().empty()) {
                if (!obj || !obj->has(it->name())) return false;
                return load(it + 1, obj->value(it->name()), path, it->name(), it->name());
            }
            else {
				if (!obj) return false;
				_stack.push(std::make_tuple(it, obj->iterateValues(), path));
				return processNext(static_cast<int>(_stack.size()));
            }
        }

	    GwtQueryIterator& GwtQueryIterator::operator++()
	    {
			processNext(0);
			return *this;
		}

	    bool GwtQueryIterator::processNext(int tail)
		{
			auto deepAvailable = true;
			auto advance = true;
			if (tail > 0) {
				tail--;
				advance = false;
			}
			while (true) {
                if (_stack.size() == tail) {
                    _current.path = "";
                    _current.value = GwtValue();
                    break;
                }
                // Текущий кусок пути
				auto child = std::get<0>(_stack.top());
                // Итератор внутри текущего куска пути
				auto iterator = std::get<1>(_stack.top());
                // Путь к итерируемой ноде
				auto path = std::get<2>(_stack.top());

				if (advance) {
					auto current = iterator->get().toObject();
					if (child->deep() && current && deepAvailable) {
						path = path + "/" + iterator->name();
						iterator = current->iterateValues();
						_stack.push(std::make_tuple(child, iterator, path));
					}
					else {
						iterator->advance();
					}
				}

				if (iterator->empty()) {
					_stack.pop();
					deepAvailable = false;
				}
				else {
					deepAvailable = true;
					if (!child->test() || child->test()->match(iterator->get(), iterator->part(), _values)) {
						if (load(child + 1, iterator->get(), path, iterator->part(), iterator->name())) return true;
					}
				}
				advance = true;
			}
			return false;
		}

	    bool GwtQueryIterator::operator==(const GwtQueryIterator& rhs) const
	    {
			return _current.path == rhs._current.path;
		}

	    bool GwtQueryIterator::operator!=(const GwtQueryIterator& rhs) const
	    {
			return !(*this == rhs); 
		}

		GwtQueryIterable &GwtQueryIterator::operator*()
		{
			return _current;
		}

		GwtQueryIterable *GwtQueryIterator::operator->()
		{
			return &_current;
		}

		const GwtQueryIterable &GwtQueryIterator::operator*() const
	    {
			return _current;
		}

		const GwtQueryIterable *GwtQueryIterator::operator->() const
		{
			return &_current;
		}

		GwtQuery::GwtQuery(const GwtValue &value,
			const std::string &path, std::initializer_list<GwtValue> &&values)
				: _value(value),
			      _path(GwtPathExpression::parse(path)),
			      _values(std::move(values))

	    {}

	    GwtQuery::GwtQuery(const GwtValue &value,
			const GwtPath &path, std::initializer_list<GwtValue> &&values)
				: _value(value),
				  _path(path),
				  _values(std::move(values))
		{}

	    GwtQueryIterator GwtQuery::begin() const
	    {
			return GwtQueryIterator(_path, _value, _values, "");
	    }

	    GwtQueryIterator GwtQuery::end() const
	    {
			return GwtQueryIterator();
	    }

		GwtValue GwtQuery::first()
	    {
			for(auto pair: *this) {
				return pair.value;
			}
			return GwtValue();
	    }
    }
}
