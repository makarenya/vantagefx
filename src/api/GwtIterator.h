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
#include "GwtPathExpression.h"
#include "GwtValue.h"
#include <stack>
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

            virtual GwtIteratorPtr clone();

            virtual GwtValue get() { return GwtValue(); }

			virtual GwtFieldPtr field() { return GwtFieldPtr(); }

			virtual std::string path() const { return ""; }

            static GwtIteratorPtr createIterator(std::shared_ptr<GwtObject> &object,
                                                 GwtPath::const_iterator it,
                                                 GwtPath::const_iterator end,
												 std::string path);
        };

        class GwtSingleIterator : public GwtIterator {
        public:
            GwtSingleIterator(const GwtValue &value, const GwtFieldPtr &field, std::string path);

            void advance() override;

            bool empty() const override;

            GwtIteratorPtr clone() override;

			GwtValue get() override;

			GwtFieldPtr field() override;

			std::string path() const override { return _path; }

		private:
			GwtValue _value;
			GwtFieldPtr _field;
			std::string _path;
        };

        class GwtMultiIterator : public GwtIterator {
        public:
            void advance() override;

            bool empty() const override;

			GwtValue get() override;

			GwtFieldPtr field() override;

			std::string path() const override;
		
		protected:
            GwtMultiIterator(GwtPath::const_iterator it, GwtPath::const_iterator end);

            GwtMultiIterator(GwtPath childPath, GwtPath filterPath,
                             const GwtValue &filterValue, GwtIteratorPtr child);

            void resetChild();

            virtual void advanceInternal() = 0;

            virtual bool internalEnd() const = 0;

            virtual GwtValue internalGet() = 0;

			virtual GwtFieldPtr internalField() = 0;

			virtual std::string internalPath() = 0;

            GwtPath const &childPath() const { return _childPath; }

            GwtPath const &filterPath() const { return _filterPath; }

            const GwtValue &filterValue() const { return _filterValue; }

            GwtIteratorPtr child() const { return _child->clone(); }
		
		private:
            GwtPath _childPath;
            GwtPath _filterPath;
			GwtValue _filterValue;
            GwtIteratorPtr _child;
		};

        class GwtComplexIterator : public GwtMultiIterator {
        public:
            typedef std::vector<std::shared_ptr<GwtField>>::iterator InternalIterator;

			GwtComplexIterator(std::shared_ptr<GwtObject> &object,
				InternalIterator fieldsBegin, InternalIterator fieldsEnd,
                GwtPath::const_iterator it, GwtPath::const_iterator end, std::string path);

			GwtComplexIterator(GwtPath childPath, GwtPath filterPath,
				const GwtValue &filterValue, GwtIteratorPtr child,
				std::shared_ptr<GwtObject> &object,
                InternalIterator it, InternalIterator end, std::string path);

            void advanceInternal() override;

            bool internalEnd() const override;

			GwtValue internalGet() override;

			GwtFieldPtr internalField() override;

			std::string internalPath() override;

			GwtIteratorPtr clone() override;

		private:
			std::string _path;
			InternalIterator _it;
            InternalIterator _end;
			std::shared_ptr<GwtObject> _object;
        };

		class GwtMapIterator : public GwtMultiIterator {
		public:
			typedef std::map<std::string, GwtValue>::iterator InternalIterator;

			GwtMapIterator(std::shared_ptr<GwtObject> &object,
				GwtPath::const_iterator it,
				GwtPath::const_iterator end, std::string path,
				const GwtFieldPtr &field);

			GwtMapIterator(GwtPath childPath, GwtPath filterPath,
				const GwtValue &filterValue, GwtIteratorPtr child,
				InternalIterator it, InternalIterator end, std::string path,
				const GwtFieldPtr &field);

			void advanceInternal() override;

			bool internalEnd() const override;

			GwtValue internalGet() override;

			GwtFieldPtr internalField() override;

			std::string internalPath() override;

			GwtIteratorPtr clone() override;

		private:
			std::string _path;
			GwtFieldPtr _field;
			InternalIterator _it;
			InternalIterator _end;
		};

		class GwtDeepIterator : public GwtMultiIterator {
        public:
            GwtDeepIterator(std::shared_ptr<GwtObject> &object,
                            GwtPath::const_iterator it,
                            GwtPath::const_iterator end, std::string path);

            GwtDeepIterator(GwtPath childPath, GwtPath filterPath,
				const GwtValue &filterValue, GwtIteratorPtr child,
                std::deque<GwtIteratorPtr> deep);

            void advanceInternal() override;

            bool internalEnd() const override;

			GwtValue internalGet() override;

			GwtFieldPtr internalField() override;

			std::string internalPath() override;

			GwtIteratorPtr clone() override;

		private:
            std::stack<GwtIteratorPtr> _deep;
			GwtPath _subpath;
        };

        class GwtArrayIterator : public GwtMultiIterator {
        public:
            GwtArrayIterator(std::shared_ptr<GwtObject> &object,
                GwtPath::const_iterator it, GwtPath::const_iterator end, 
				std::string path, const GwtFieldPtr &field);

            GwtArrayIterator(GwtPath childPath, GwtPath filterPath,
				const GwtValue &filterValue, GwtIteratorPtr child,
                std::shared_ptr<GwtObject> &object,
                int index, int length, std::string path, const GwtFieldPtr &field);

            void advanceInternal() override;

            bool internalEnd() const override;

			GwtValue internalGet() override;

			GwtFieldPtr internalField() override;

			std::string internalPath() override;

			GwtIteratorPtr clone() override;

        private:
			std::string _path;
			std::shared_ptr<GwtObject> _object;
			GwtFieldPtr _field;
            int _index;
            int _length;
        };

		struct GwtQueryIterable
		{
			std::string path;
			std::shared_ptr<GwtField> field;
			GwtValue value;
		};

        class GwtQueryIterator : std::iterator<std::input_iterator_tag, GwtQueryIterable> {
        public:
            explicit GwtQueryIterator(GwtIteratorPtr &x);

            GwtQueryIterator(const GwtQueryIterator &mit);

			void load(GwtIteratorPtr &x);

            GwtQueryIterator &operator++();

            GwtQueryIterator operator++(int);

            bool operator==(const GwtQueryIterator &rhs) const;

            bool operator!=(const GwtQueryIterator &rhs) const;

			GwtQueryIterable &operator*();
			GwtQueryIterable *operator->();
			const GwtQueryIterable &operator*() const;
			const GwtQueryIterable *operator->() const;

        private:
            GwtIteratorPtr _iterator;
			GwtQueryIterable _current;
        };

        class GwtQuery {
        public:
			typedef GwtQueryIterator iterator;
			typedef std::pair<std::string, GwtValue> value_type;

            GwtQuery(std::shared_ptr<GwtObject> &object, std::string path);

            GwtQuery(std::shared_ptr<GwtObject> &object, GwtPath path);

            GwtQuery(std::shared_ptr<GwtObject> &object, GwtPath::const_iterator it, GwtPath::const_iterator end);

            GwtQueryIterator begin();

            GwtQueryIterator end();

			GwtValue first();

        private:
            std::shared_ptr<GwtObject> _object;
            GwtPath _path;
			GwtValue _value;
        };

    }
}


#endif //VANTAGEFX_GWTITERATOR_H
