//
// Created by alexx on 29.04.2016.
//

#ifndef VANTAGEFX_GWTHTTPREQUESTCONTEXT_H
#define VANTAGEFX_GWTHTTPREQUESTCONTEXT_H

#include <stdint.h>
#include <string>
#include <vector>

namespace vantagefx {
    namespace api {

        class GwtHttpRequestContext
        {
        public:
            GwtHttpRequestContext &operator<<(int value);
            GwtHttpRequestContext &operator<<(int64_t value);
            GwtHttpRequestContext &operator<<(double value);
            GwtHttpRequestContext &operator<<(std::string value);
            std::string str() const;
        private:
            std::vector<std::string> _strings;
            std::vector<std::string> _data;
        };
    }
}


#endif //VANTAGEFX_GWTHTTPREQUESTCONTEXT_H
