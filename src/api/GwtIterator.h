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

            virtual GwtValuePtr &get() { return _empty; }

			virtual std::string path() const { return ""; }

            static GwtIteratorPtr createIterator(std::shared_ptr<GwtObject> &object,
                                                 GwtPath::const_iterator it,
                                                 GwtPath::const_iterator end,
												 std::string path);

        private:
            GwtValuePtr _empty;
        };

        class GwtSingleIterator : public GwtIterator {
        public:
            explicit GwtSingleIterator(GwtValuePtr &value, std::string path);

            void advance() override;

            bool empty() const override;

            GwtIteratorPtr clone() override;

            GwtValuePtr &get() override;

			std::string path() const override { return _path; }

		private:
            GwtValuePtr _value;
			std::string _path;
        };

        class GwtMultiIterator : public GwtIterator {
        public:
            void advance() override;

            bool empty() const override;

            GwtValuePtr &get() override;

			std::string path() const override;
		
		protected:
            GwtMultiIterator(GwtPath::const_iterator it, GwtPath::const_iterator end);

            GwtMultiIterator(GwtPath childPath, GwtPath filterPath,
                             std::string filterValue, GwtIteratorPtr child);

            void resetChild();

            virtual void advanceInternal() = 0;

            virtual bool internalEnd() const = 0;

            virtual GwtValuePtr &internalGet() = 0;

			virtual std::string internalPath() = 0;

            GwtPath const &childPath() const { return _childPath; }

            GwtPath const &filterPath() const { return _filterPath; }

            std::string filterValue() const { return _filterValue; }

            GwtIteratorPtr child() const { return _child->clone(); }
		
		private:
            GwtPath _childPath;
            GwtPath _filterPath;
            std::string _filterValue;
            GwtIteratorPtr _child;
		};

        class GwtMapIterator : public GwtMultiIterator {
        public:
            typedef std::map<std::string, std::shared_ptr<GwtValue>>::iterator InternalIterator;

            GwtMapIterator(std::shared_ptr<GwtObject> &object,
                           GwtPath::const_iterator it,
                           GwtPath::const_iterator end, std::string path);

            GwtMapIterator(GwtPath childPath, GwtPath filterPath,
                           std::string filterValue, GwtIteratorPtr child,
                           InternalIterator it, InternalIterator end, std::string path);

            void advanceInternal() override;

            bool internalEnd() const override;

            GwtValuePtr &internalGet() override;

			std::string internalPath() override;

			GwtIteratorPtr clone() override;

		private:
			std::string _path;
			InternalIterator _it;
            InternalIterator _end;
        };

        class GwtDeepIterator : public GwtMultiIterator {
        public:
            GwtDeepIterator(std::shared_ptr<GwtObject> &object,
                            GwtPath::const_iterator it,
                            GwtPath::const_iterator end, std::string path);

            GwtDeepIterator(GwtPath childPath, GwtPath filterPath,
                            std::string filterValue, GwtIteratorPtr child,
                            std::deque<GwtIteratorPtr> deep);

            void advanceInternal() override;

            bool internalEnd() const override;

            GwtValuePtr &internalGet() override;

			std::string internalPath() override;

			GwtIteratorPtr clone() override;

		private:
            std::stack<GwtIteratorPtr> _deep;
			GwtPath _subpath;
        };

        class GwtArrayIterator : public GwtMultiIterator {
        public:
            GwtArrayIterator(std::shared_ptr<GwtObject> &object,
                             GwtPath::const_iterator it,
                             GwtPath::const_iterator end, std::string path);

            GwtArrayIterator(GwtPath childPath, GwtPath filterPath,
                             std::string filterValue, GwtIteratorPtr child,
                             std::shared_ptr<GwtObject> &object,
                             int index, int length, std::string path);

            void advanceInternal() override;

            bool internalEnd() const override;

            GwtValuePtr &internalGet() override;

			std::string internalPath() override;

			GwtIteratorPtr clone() override;

        private:
			std::string _path;
			std::shared_ptr<GwtObject> _object;
            int _index;
            int _length;
        };

        class GwtQueryIterator : std::iterator<std::input_iterator_tag, std::pair<std::string, GwtObject&>> {
        public:
            explicit GwtQueryIterator(GwtIteratorPtr &x);

            GwtQueryIterator(const GwtQueryIterator &mit);

            GwtQueryIterator &operator++();

            GwtQueryIterator operator++(int);

            bool operator==(const GwtQueryIterator &rhs) const;

            bool operator!=(const GwtQueryIterator &rhs) const;

			std::pair<std::string, GwtValue&> operator*() const;

        private:
            GwtIteratorPtr _iterator;
        };

        class GwtQuery {
        public:
			typedef GwtQueryIterator iterator;
			typedef std::pair<std::string, GwtValue&> value_type;

            GwtQuery(std::shared_ptr<GwtObject> &object, std::string path);

            GwtQuery(std::shared_ptr<GwtObject> &object, GwtPath path);

            GwtQuery(std::shared_ptr<GwtObject> &object, GwtPath::const_iterator it, GwtPath::const_iterator end);

            GwtQueryIterator begin();

            GwtQueryIterator end();

        private:
            std::shared_ptr<GwtObject> _object;
            GwtPath _path;
        };

    }
}


#endif //VANTAGEFX_GWTITERATOR_H
