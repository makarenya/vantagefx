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

		template<typename T>
		GwtValue ConvertPathTo(const GwtValue &item, const std::string &, const std::string &part)
		{
			try {
				return GwtValue(boost::lexical_cast<T>(part));
			}
			catch(boost::bad_lexical_cast) {
				return GwtValue();
			}
		}

	    GwtQueryIterator::GwtQueryIterator(const GwtPath &path, const GwtValue &value,
			const std::vector<GwtValue> &values, const std::string &prefix)
			: _path(path),
			  _values(values)
	    {
			using namespace std::placeholders;
            _methods["int"] = std::bind(&ConvertPathTo<int>, _1, _2, _3);
            _methods["long"] = std::bind(&ConvertPathTo<int64_t>, _1, _2, _3);
            _methods["double"] = std::bind(&ConvertPathTo<double>, _1, _2, _3);

            _methods["key"] = [this](const GwtValue &item, const std::string &, const std::string &part) {
                return GwtValue(part);
            };
            _methods["to_string"] = [this](const GwtValue &item, const std::string &, const std::string &part) {
                return GwtValue(item.toString());
            };
            _methods["to_int"] = [this](const GwtValue &item, const std::string &, const std::string &part) {
                return GwtValue(item.toInt());
            };
            _methods["to_long"] = [this](const GwtValue &item, const std::string &, const std::string &part) {
                return GwtValue(item.toLong());
            };
            _methods["to_double"] = [this](const GwtValue &item, const std::string &, const std::string &part) {
                return GwtValue(item.toDouble());
            };

            if (load(_path.begin(), value, "", prefix, prefix) == 2) {
				processNext(false);
			}
		}

	    GwtQueryIterator::GwtQueryIterator(GwtQueryIterator &&rhs)
			: _path(std::move(rhs._path)),
			  _stack(std::move(rhs._stack)),
			  _current(std::move(rhs._current)),
			  _methods(std::move(rhs._methods))
		{ }

	    GwtQueryIterator &GwtQueryIterator::operator=(GwtQueryIterator &&rhs)
	    {
			if (this == &rhs) return *this;
			_path = std::move(rhs._path);
			_stack = std::move(rhs._stack);
			_current = std::move(rhs._current);
			_methods = std::move(rhs._methods);
			return *this;
		}

	    void GwtQueryIterator::set(const GwtValue &value, const std::string &path)
		{
			_current.value = value;
			_current.path = path;
		}

        int GwtQueryIterator::load(GwtPath::iterator it, const GwtValue &item, std::string path, const std::string &part, const std::string &name)
        {
			if (!path.empty()) path += "/" + name;
			else path = name;
			if (it == _path.end()) {
				set(item, path);
				return 1;
			}
			auto nameLength = it->name().length();
			if (nameLength > 2 && it->name().find("()") == nameLength - 2) {
				set(_methods.at(it->name().substr(0, nameLength - 2))(item, path, part), path);
				return 1;
			}
			auto obj = item.toObject();
			if (!it->name().empty()) {
				if (!obj || !obj->has(it->name())) return 0;
				return load(it + 1, obj->value(it->name()), path, it->name(), it->name());
			}
			
			if (!obj) return 0; 
			auto iterator = obj->iterateValues();
			if (iterator->empty()) return 0;
			_stack.push(std::make_tuple(it, iterator, path));
			return 2;
        }

	    GwtQueryIterator& GwtQueryIterator::operator++()
	    {
			processNext(true);
			return *this;
		}

	    bool GwtQueryIterator::processNext(bool advance)
		{
			auto deepAvailable = true;

			while (true) {
                if (_stack.size() == 0) {
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
						path = path.empty() ? iterator->name() : path + "/" + iterator->name();
						iterator = current->iterateValues();
						_stack.push(std::make_tuple(child, iterator, path));
					}
					else {
						iterator->advance();
					}
				}

				advance = true;
				deepAvailable = true;

				if (iterator->empty()) {
					if (child->deep()) deepAvailable = false;
					_stack.pop();
				}
				else {
					if (!child->test() || child->test()->match(iterator->get(), iterator->part(), _values)) {
						auto result = load(child + 1, iterator->get(), path, iterator->part(), iterator->name());
						if (result == 1) return true;
						if (result == 2) advance = false;
					}
				}
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
