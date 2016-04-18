//
// Created by alexx on 17.04.2016.
//

#ifndef VANTAGEFX_URL_H
#define VANTAGEFX_URL_H

#include <string>

namespace vantagefx {
    namespace http {

        class Url
        {
        public:

            Url();

            explicit Url(std::string url);

            const std::string &scheme() const { return _scheme; }

            const std::string &host() const { return _host; }

            int port() const { return _port; };

            const std::string &path() const { return _path; }

            const std::string &hash() const { return _hash; }

            const std::string &uri() const { return _uri; }

            std::string serverUrl() const;

            operator std::string() const;

        private:
            std::string _scheme;
            std::string _host;
            int _port;
            std::string _path;
            std::string _hash;
            std::string _uri;
        };
    }
}

#endif //VANTAGEFX_URL_H
