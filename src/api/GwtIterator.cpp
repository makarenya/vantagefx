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


		GwtQueryIterator::GwtQueryIterator(const GwtPath &path, const GwtConstObjectPtr &object,
			const std::vector<GwtValue> &values, const std::string &prefix)
			: _path(path),
			  _values(values)
	    {
            load(_path.begin(), GwtValue(object), "", prefix);
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

		bool GwtQueryIterator::loadLevel(const GwtIteratorPtr &iterator, GwtPath::iterator it, 
			const std::string &path)
		{
			auto current =  iterator->get().toObject();
			if (it->test()) {
				if (!current) return false;
				if (!it->test()->match(current, iterator->part(), _values)) return false;
			}
			return load(it + 1, iterator->get(), path, iterator->part());
		}

        bool GwtQueryIterator::load(GwtPath::iterator it, const GwtValue &item, std::string path, const std::string &part)
        {
			if (!path.empty()) path += "/" + part;
			else path = part;
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

			if (!it->name().empty()) {
                auto obj = item.toObject();
                if (!obj || !obj->has(it->name())) return false;
                return load(it + 1, obj->value(it->name()), path, it->name());
            }
            else {
                auto obj = item.toObject();
				if (!obj) return false;
                for(auto iterator = obj->iterateValues(); !iterator->empty(); iterator->advance()) {
					if (loadLevel(iterator, it, path)) {
						_stack.push(std::make_tuple(it, iterator, path));
						return true;
					}
                }
                return false;
            }
        }

	    GwtQueryIterator& GwtQueryIterator::operator++()
	    {
		    auto deepAvailable = true;
            while(true) {
				if (_stack.empty()) {
					_current.path = "";
					_current.value = GwtValue();
					break;
				}
                auto child = std::get<0>(_stack.top());
                auto iterator = std::get<1>(_stack.top());
                auto path = std::get<2>(_stack.top());

				auto current =  iterator->get().toObject();
                if (child->deep() && current && deepAvailable) {
					path = path + "/" + iterator->part();
					iterator = current->iterateValues();
					_stack.push(std::make_tuple(child, iterator, path));
                }
                else {
					iterator->advance();
				}

				if (iterator->empty()) {
					_stack.pop();
					deepAvailable = false;
				}
				else {
					deepAvailable = true;
					if (loadLevel(iterator, child, path)) break;
				}
            }
			return *this;
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

		GwtQuery::GwtQuery(const std::shared_ptr<const GwtObject>& object, 
			const std::string &path, std::initializer_list<GwtValue> &&values)
			: _object(object),
			  _path(GwtPathExpression::parse(path)),
			  _values(std::move(values))

	    {}

	    GwtQuery::GwtQuery(const std::shared_ptr<const GwtObject>& object, 
			const GwtPath &path, std::initializer_list<GwtValue> &&values)
			: _object(object),
			  _path(path),
			  _values(std::move(values))
		{}

	    GwtQueryIterator GwtQuery::begin() const
	    {
			return GwtQueryIterator(_path, _object, _values, "");
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
