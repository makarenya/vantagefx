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

	    GwtIteratorPtr GwtIterator::clone()
	    {
			return std::make_shared<GwtIterator>();
	    }

	    GwtIteratorPtr GwtIterator::createIterator(std::shared_ptr<GwtObject>& object,
												   GwtPath::const_iterator it,
												   GwtPath::const_iterator end,
												   std::string path)
		{
			auto name = it->name();
			if (path.empty()) path = name;
			else path += "/" + name;
			
			if (it->deep())
                return std::make_shared<GwtDeepIterator>(object, it, end, path);
			if (name.empty())
				return object->type()->iterateValues(object, it, end, path);

			auto val = object->value(it->name());
			if (++it == end) return std::make_shared<GwtSingleIterator>(val, path);
			if (!val || !val->toObject()) return std::make_shared<GwtIterator>();
			auto obj = val->toObject();
			return createIterator(obj, it, end, path);
		}

	    GwtSingleIterator::GwtSingleIterator(GwtValuePtr &value, std::string path)
			: _value(value),
			  _path(path) { }
		
		void GwtSingleIterator::advance()
	    {
			_value = GwtValuePtr();
		}

	    bool GwtSingleIterator::empty() const
	    {
			return !_value;
	    }

	    GwtIteratorPtr GwtSingleIterator::clone()
	    {
			return std::make_shared<GwtSingleIterator>(_value, _path);
	    }

	    GwtValuePtr &GwtSingleIterator::get()
	    {
			return _value;
		}

		GwtMultiIterator::GwtMultiIterator(GwtPath::const_iterator it, GwtPath::const_iterator end)
				: _filterPath(it->filterPath()),
				  _filterValue(it->filterValue())
		{
			_childPath = GwtPath(++it, end);
		}

		GwtMultiIterator::GwtMultiIterator(GwtPath childPath, GwtPath filterPath,
										   std::string filterValue, GwtIteratorPtr child)
				: _childPath(childPath),
				  _filterPath(filterPath),
				  _filterValue(filterValue),
				  _child(child)
		{}

		void GwtMultiIterator::resetChild()
		{
            // Святая святых итератора множества. Метод заменяет дочерний итератор.
            // У любого итератора множества должен быть дочерний итератор.
            // Если взять путь hello/*/world/[name='beer'], то у первого итератора множества "hello/*"
            // Дочерним будет второй итератор множества world/[name='beer'], а у второго дочерним будет
            // итератор единичного значения. То есть итератор, который возврацает одно единственной значение
            // (и это будет объект, с параметром name = 'beer')

            // Методы internalEnd, advanceInternal, internal пролистывают коллекцию дочерних значений
            // объекта, для которго создани итератор
			for (;!internalEnd(); advanceInternal()) {
				auto value = internalGet();
                // Пустые значения пролистываются
				if (!value) continue;
				auto obj = value->toObject();
                // Если итератор задан символом * (то есть не имеет фильтрующего условия)
				if (_filterPath.size() == 0) {
                    // Если нет дочернего пути (путь заканчивается на звёздочку), то просто возвращаем
                    // текущее дочернее значение
					if (_childPath.size() == 0) {
						auto child = std::make_shared<GwtSingleIterator>(value, internalPath());
						auto strval = value->toString();
						if (_filterValue.empty() || strval == _filterValue) {
							_child = child;
							return;
						}
					}
                    // Если дочерний путь есть, то проверяем что текущее дочернее значение содержит объект
					if (obj && _filterValue.empty()) {
                        // Если да, то создаём новый итератор по этому значению
						_child = createIterator(obj, _childPath.begin(), _childPath.end(), internalPath());
                        // И если он ещё и не пустым оказался, то завершаем объход дочерних значений
						if (!_child->empty()) return;
					}
					continue;
				}
                // Если задано фильтрующее условие, то текущее дочернее значение должно содержать объект, иначе
                // невозможно производить фильтрацию
				if (!obj) continue;

                // Все объекты, соответствующие пути фильтрующего условия вытаскиваются
				GwtQuery query(obj, _filterPath);
				for (auto it = query.begin(); it != query.end(); ++it) {
                    // И проверяются на соответствие фильтрующему значению
                    if ((*it).second.toString() != _filterValue) continue;

                    // Если фильрующее условие выполнено, то действия аналогичны объходу без фильтрации
                    if (_childPath.size() == 0) {
                        _child = std::make_shared<GwtSingleIterator>(value, internalPath());
                        return;
                    }
                    _child = createIterator(obj, _childPath.begin(), _childPath.end(), internalPath());
                    if (!_child->empty()) return;
                }
			}
		}

		void GwtMultiIterator::advance()
		{
			_child->advance();
			while (_child->empty() && !internalEnd()) {
				advanceInternal();
				resetChild();
			}
		}

		bool GwtMultiIterator::empty() const
		{
			if (!_child) return true;
			return _child->empty() && internalEnd();
		}

		GwtValuePtr &GwtMultiIterator::get()
		{
			return _child->get();
		}

	    std::string GwtMultiIterator::path() const
	    {
			if (empty()) return "";
			return _child->path();
	    }

	    GwtMapIterator::GwtMapIterator(std::shared_ptr<GwtObject>& object, 
			GwtPath::const_iterator it, GwtPath::const_iterator end, 
			std::string path)
			: GwtMultiIterator(it, end),
			_path(path),
			_it(object->values().begin()),
			_end(object->values().end())
		{
			resetChild();
		}

		GwtMapIterator::GwtMapIterator(GwtPath childPath, GwtPath filterPath,
			std::string filterValue, GwtIteratorPtr child,
			InternalIterator it, InternalIterator end, std::string path)
			: GwtMultiIterator(childPath, filterPath, filterValue, child),
			_path(path),
			_it(it),
			_end(end)
		{}

		
		void GwtMapIterator::advanceInternal()
	    {
		    ++_it;
	    }

	    bool GwtMapIterator::internalEnd() const
	    {
			return (_it == _end);
	    }

		GwtValuePtr &GwtMapIterator::internalGet()
		{
			return _it->second;
		}

		std::string GwtMapIterator::internalPath()
	    {
			return _path + _it->first;
	    }
		
		GwtIteratorPtr GwtMapIterator::clone()
	    {
			return std::make_shared<GwtMapIterator>(childPath(), filterPath(), filterValue(), child(), _it, _end, _path);
	    }

	    GwtDeepIterator::GwtDeepIterator(std::shared_ptr<GwtObject>& object, GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path)
			: GwtMultiIterator(it, end),
			  _subpath({ GwtPathExpression(false) })
		{
			auto iterator = object->type()->iterateValues(object, _subpath.begin(), _subpath.end(), path);
			if (!iterator->empty()) _deep.push(iterator);
			resetChild();
		}

	    GwtDeepIterator::GwtDeepIterator(GwtPath childPath, GwtPath filterPath, std::string filterValue, GwtIteratorPtr child, std::deque<GwtIteratorPtr> deep)
			: GwtMultiIterator(childPath, filterPath, filterValue, child)
	    {
            for(auto it: deep) {
                _deep.push(it->clone());
            }
		}

	    void GwtDeepIterator::advanceInternal()
	    {
			// Если есть возможность спуститься глубже, то производится спуск на 1 уровень
			auto obj = _deep.top()->get()->toObject();
			if (obj) {
				// Создаётся итератор для дочернего уровня
				auto iterator = obj->type()->iterateValues(obj, _subpath.begin(), _subpath.end(), _deep.top()->path() + "/");
				while (!iterator->empty() && !iterator->get()->toObject())
					iterator->advance();
				if (!iterator->empty()) {
					_deep.push(iterator);
					return;
				}
			}
			// Текущий верхний уровень итераторов передвигаем на 1
			auto iterator = _deep.top();
			iterator->advance();
			while (!iterator->empty() && !iterator->get()->toObject())
				iterator->advance();

			while (iterator->empty()) {
				_deep.pop();
				if (_deep.empty()) return;
				iterator = _deep.top();
				iterator->advance();
				while (!iterator->empty() && !iterator->get()->toObject())
					iterator->advance();
			}
		}

	    bool GwtDeepIterator::internalEnd() const
	    {
			return _deep.empty();
	    }

		GwtValuePtr& GwtDeepIterator::internalGet()
	    {
			return _deep.top()->get();
	    }

		std::string GwtDeepIterator::internalPath()
	    {
			return _deep.top()->path();
	    }

		GwtIteratorPtr GwtDeepIterator::clone()
	    {
            std::deque<GwtIteratorPtr> iterators;
            while(!_deep.empty()) {
                iterators.push_front(_deep.top());
                _deep.pop();
            }
			for(auto it: iterators) {
                _deep.push(it);
			}
		    return std::make_shared<GwtDeepIterator>(childPath(), filterPath(), filterValue(), child(), iterators);
	    }

	    GwtArrayIterator::GwtArrayIterator(std::shared_ptr<GwtObject>& object, GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path)
			: GwtMultiIterator(it, end),
			  _path(path),
			  _object(object),
			  _index(0),
			  _length(object->value("length")->intValue())
	    {
			resetChild();
	    }

	    void GwtArrayIterator::advanceInternal()
	    {
			_index++;
	    }

	    bool GwtArrayIterator::internalEnd() const
	    {
			return _index == _length;
	    }

	    GwtValuePtr &GwtArrayIterator::internalGet()
	    {
			return _object->value(boost::lexical_cast<std::string>(_index));
	    }

		std::string GwtArrayIterator::internalPath()
	    {
			auto obj = internalGet()->toObject();
			if (obj && !obj->type()->primary().empty()) {
				return _path + "[" + obj->type()->primary() + "='" + obj->primary() + "']";
			}
			return _path + boost::lexical_cast<std::string>(_index);
	    }

        GwtIteratorPtr GwtArrayIterator::clone() {
            return std::make_shared<GwtArrayIterator>(childPath(), filterPath(), filterValue(), child(),
                                                      _object, _index, _length, _path);
        }

        GwtArrayIterator::GwtArrayIterator(GwtPath childPath, GwtPath filterPath, std::string filterValue,
                                           GwtIteratorPtr child, std::shared_ptr<GwtObject> &object, int index,
                                           int length, std::string path)
                : GwtMultiIterator(childPath, filterPath, filterValue, child),
				  _path(path),
                  _object(object),
                  _index(index),
                  _length(length)
        {}

	    GwtQueryIterator::GwtQueryIterator(GwtIteratorPtr& x)
			:_iterator(x)
	    {}

	    GwtQueryIterator::GwtQueryIterator(const GwtQueryIterator& mit)
			: _iterator(mit._iterator->clone())
	    {}

	    GwtQueryIterator& GwtQueryIterator::operator++()
	    {
			_iterator->advance();
			std::string result = _iterator->path();
			return *this; 
		}

	    GwtQueryIterator GwtQueryIterator::operator++(int)
	    {
			auto tmp(*this); 
			operator++(); 
			return tmp; 
		}

	    bool GwtQueryIterator::operator==(const GwtQueryIterator& rhs) const
	    {
			if (_iterator->empty() && rhs._iterator->empty()) return true;
			if (_iterator->empty() || rhs._iterator->empty()) return false;
			return _iterator->get().get() == rhs._iterator->get().get();
		}

	    bool GwtQueryIterator::operator!=(const GwtQueryIterator& rhs) const
	    {
			return !(*this == rhs); 
		}

		std::pair<std::string, GwtValue&> GwtQueryIterator::operator*() const
	    {
			return std::pair<std::string, GwtValue&>(_iterator->path(), *_iterator->get());
		}

	    GwtQuery::GwtQuery(std::shared_ptr<GwtObject>& object, std::string path)
			: _object(object),
			  _path(GwtPathExpression::parse(path))
	    {}

	    GwtQuery::GwtQuery(std::shared_ptr<GwtObject>& object, GwtPath path)
			: _object(object),
			  _path(path)
		{}

	    GwtQuery::GwtQuery(std::shared_ptr<GwtObject>& object, GwtPath::const_iterator it, GwtPath::const_iterator end)
			: _object(object),
			  _path(it, end)
		{}

	    GwtQueryIterator GwtQuery::begin()
	    {
		    auto iterator = GwtIterator::createIterator(_object, _path.cbegin(), _path.cend(), "");
			return GwtQueryIterator(iterator);
	    }

	    GwtQueryIterator GwtQuery::end()
	    {
		    auto iterator = std::make_shared<GwtIterator>();
			return GwtQueryIterator(iterator);
	    }
    }
}