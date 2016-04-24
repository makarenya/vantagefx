//
// Created by AMakarenya on 04.04.2016.
//

#ifndef VANTAGEFX_GWTPATHEXPRESSION_H
#define VANTAGEFX_GWTPATHEXPRESSION_H

#include <vector>
#include <string>
#include <memory>
#include "GwtValue.h"

namespace vantagefx {
    namespace api {

        class GwtPathExpression;

        typedef std::vector<GwtPathExpression> GwtPath;

        class GwtPathExpression {
        public:
			GwtPathExpression()
				: _deep(false) { }

			explicit GwtPathExpression(bool deep);

            explicit GwtPathExpression(std::string name);

            GwtPathExpression(GwtPath filterPath, GwtValue filterValue);

            static GwtPath parse(std::string path);

            std::string const &name() const { return _name; }
            bool deep() const { return _deep; }
            std::vector<GwtPathExpression> const &filterPath() const { return _filterPath; }
			GwtValue const &filterValue() const { return _filterValue; }

        private:
            bool _deep;
            std::string _name;
            GwtPath _filterPath;
			GwtValue _filterValue;
        };
    }
}


#endif //VANTAGEFX_GWTPATHEXPRESSION_H
