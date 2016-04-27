//
// Created by AMakarenya on 04.04.2016.
//

#ifndef VANTAGEFX_GWTPATHEXPRESSION_H
#define VANTAGEFX_GWTPATHEXPRESSION_H

#include <vector>
#include <string>
#include <memory>
#include <boost/optional/optional.hpp>
#include "GwtValue.h"

namespace vantagefx {
    namespace api {

        class GwtPathExpression;

        typedef std::vector<GwtPathExpression> GwtPath;

        class GwtValueRef {
        public:

            explicit GwtValueRef(int num);

            GwtValueRef();

            explicit GwtValueRef(const GwtValue &value);

            GwtValue operator()(const std::vector<GwtValue> &values) const;

        private:
            int _num;
            GwtValue _value;
        };

        class GwtPathTest 
		{
        public:
			virtual ~GwtPathTest() {}
            virtual bool match(const std::shared_ptr<const GwtObject> &object, const std::string &prefix,
                       const std::vector<GwtValue> &values) const = 0;
        };

        typedef std::shared_ptr<GwtPathTest> GwtPathTestPtr;

        class GwtPathTestOr : public GwtPathTest
        {
        public:

            GwtPathTestOr(const GwtPathTestPtr& left, const GwtPathTestPtr& right);

            bool match(const std::shared_ptr<const GwtObject>& object, const std::string& prefix,
				const std::vector<GwtValue> &values) const override;

        private:
            GwtPathTestPtr _left;
            GwtPathTestPtr _right;
        };

        class GwtPathTestAnd : public GwtPathTest
        {
        public:

            GwtPathTestAnd(const GwtPathTestPtr& left, const GwtPathTestPtr& right);

            bool match(const std::shared_ptr<const GwtObject>& object, const std::string& prefix,
				const std::vector<GwtValue> &values) const override;

        private:
            GwtPathTestPtr _left;
            GwtPathTestPtr _right;
        };

        class GwtPathTestEq : public GwtPathTest
        {
        public:

            GwtPathTestEq(GwtPath path, GwtValueRef value);

            bool match(const std::shared_ptr<const GwtObject>& object, const std::string& prefix,
				const std::vector<GwtValue> &values) const override;

        private:
            GwtPath _path;
            GwtValueRef _value;
        };

        class GwtPathExpression {
        public:
            GwtPathExpression()
                    : _deep(false) { }

            explicit GwtPathExpression(bool deep);

            explicit GwtPathExpression(std::string name);

            explicit GwtPathExpression(const GwtPathTestPtr &test);

            static GwtPath parse(std::string path);

            std::string const &name() const { return _name; }

            bool deep() const { return _deep; }

            const GwtPathTestPtr &test() const { return _test; }

        private:
            bool _deep;
            std::string _name;
            GwtPathTestPtr _test;
        };
    }
}


#endif //VANTAGEFX_GWTPATHEXPRESSION_H
