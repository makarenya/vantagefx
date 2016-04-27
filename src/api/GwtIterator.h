//
// Created by AMakarenya on 01.04.2016.
//

#ifndef VANTAGEFX_GWTITERATOR_H
#define VANTAGEFX_GWTITERATOR_H

#include <memory>
#include <string>
#include <deque>
#include <iterator>
#include <map>
#include <stack>
#include "GwtPathExpression.h"
#include "GwtValue.h"
#include "GwtField.h"

namespace vantagefx {
    namespace api {

        class GwtObject;

        class GwtIterator;

        typedef std::shared_ptr<GwtIterator> GwtIteratorPtr;

        class GwtIterator {
        public:
            virtual ~GwtIterator() { }

            virtual void advance() { }

            virtual bool empty() const { return true; }

            virtual GwtValue get() const { return GwtValue(); }

			virtual GwtFieldPtr field() const { return GwtFieldPtr(); }

			virtual std::string part() const { return ""; }
        };

        class GwtComplexIterator : public GwtIterator {
        public:
            typedef std::vector<std::shared_ptr<GwtField>>::const_iterator InternalIterator;

			GwtComplexIterator(const std::shared_ptr<const GwtObject> &object, InternalIterator it, InternalIterator end);

            void advance() override;

            bool empty() const override;

			GwtValue get() const override;

			GwtFieldPtr field() const override;

			std::string part() const override;

		private:
			InternalIterator _it;
            InternalIterator _end;
			std::shared_ptr<const GwtObject> _object;
        };

		class GwtMapIterator : public GwtIterator {
		public:
			typedef std::map<std::string, GwtValue>::const_iterator InternalIterator;

			GwtMapIterator(InternalIterator it, InternalIterator end);

            void advance() override;

            bool empty() const override;

            GwtValue get() const override;

            GwtFieldPtr field() const override;

            std::string part() const override;

		private:
			GwtFieldPtr _field;
			InternalIterator _it;
			InternalIterator _end;
		};

        class GwtArrayIterator : public GwtIterator {
        public:
            explicit GwtArrayIterator(const std::shared_ptr<const GwtObject> &object);

            void advance() override;

            bool empty() const override;

            GwtValue get() const override;

            GwtFieldPtr field() const override;

            std::string part() const override;

        private:
			std::shared_ptr<const GwtObject> _object;
			GwtFieldPtr _field;
            int _index;
            int _length;
        };

		struct GwtQueryIterable
		{
			std::string path;
			GwtValue value;
		};

        class GwtQueryIterator : std::iterator<std::input_iterator_tag, GwtQueryIterable> {
        public:
            typedef std::tuple<GwtPath::iterator, GwtIteratorPtr, std::string> GwtPathNode;
            typedef std::stack<GwtPathNode> GwtPathStack;

            GwtQueryIterator(const GwtPath &path, const std::shared_ptr<const GwtObject> &object, 
				const std::vector<GwtValue> &values, const std::string &prefix);
			GwtQueryIterator();

			GwtQueryIterator(GwtQueryIterator &&rhs);
			GwtQueryIterator &operator=(GwtQueryIterator &&rhs);

			bool set(const GwtValue &value, const std::string &path);

			bool loadLevel(const GwtIteratorPtr &iterator, GwtPath::iterator it, 
				const std::string &pathconst);

			bool load(GwtPath::iterator it, const GwtValue &item, std::string path, const std::string &part);

            GwtQueryIterator &operator++();

            bool operator==(const GwtQueryIterator &rhs) const;

            bool operator!=(const GwtQueryIterator &rhs) const;

			GwtQueryIterable &operator*();
			GwtQueryIterable *operator->();
			const GwtQueryIterable &operator*() const;
			const GwtQueryIterable *operator->() const;

        private:

			GwtQueryIterator(const GwtQueryIterator &);
			GwtQueryIterator &operator=(const GwtQueryIterator &);

            GwtPath _path;
            GwtPathStack _stack;
			GwtQueryIterable _current;
			std::vector<GwtValue> _values;
        };

        class GwtQuery {
        public:
			typedef GwtQueryIterator iterator;
			typedef std::pair<std::string, GwtValue> value_type;

            GwtQuery(const std::shared_ptr<const GwtObject> &object, 
				const std::string &path, std::initializer_list<GwtValue> &&values = {});

            GwtQuery(const std::shared_ptr<const GwtObject> &object, 
				const GwtPath &path, std::initializer_list<GwtValue> &&values = {});

            GwtQueryIterator begin() const;

            GwtQueryIterator end() const;

			GwtValue first();

        private:
            std::shared_ptr<const GwtObject> _object;
            GwtPath _path;
			GwtValue _value;
			std::vector<GwtValue> _values;
        };

    }
}


#endif //VANTAGEFX_GWTITERATOR_H
