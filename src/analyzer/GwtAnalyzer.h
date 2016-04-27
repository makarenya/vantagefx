//
// Created by Alexx on 17.04.16.
//

#ifndef VANTAGEFX_GWTANALYZER_H
#define VANTAGEFX_GWTANALYZER_H

#include <string>
#include <boost/filesystem/path.hpp>
#include "FiddlerLogEntry.h"
#include "../api/GwtObject.h"
#include "../api/GwtBundle.h"


namespace vantagefx {
    namespace analyzer {
        using boost::filesystem::path;
        using namespace api;

        class GwtAnalyzer
        {
        public:
	        explicit GwtAnalyzer(GwtBundle &bundle);
            void processResponse(path dir, std::string name, std::string content) const;
			GwtObjectPtr processRequest(const std::string& request, QDomElement &body) const;
			GwtObjectPtr processResponse(const std::string &text, QDomElement &body) const;
	        GwtObjectPtr processEntry(path dir, std::string name, FiddlerLogEntry entry) const;
            std::vector<FiddlerLogEntry> parseEntries(path filename) const;
            std::vector<std::string> usedBy(GwtConstObjectPtr object, std::vector<std::string> ids) const;
        private:
            GwtBundle &_bundle;
        };
    }
}


#endif //VANTAGEFX_GWTANALYZER_H
