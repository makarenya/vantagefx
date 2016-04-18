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
            GwtAnalyzer(GwtBundle &bundle);
            void processResponse(path dir, std::string name, std::string content);
            GwtObjectPtr processEntry(path dir, std::string name, FiddlerLogEntry entry);
            std::vector<FiddlerLogEntry> parseEntries(path filename);
            std::vector<std::string> usedBy(GwtObjectPtr object, std::vector<std::string> ids);
        private:
            GwtBundle &_bundle;
        };
    }
}


#endif //VANTAGEFX_GWTANALYZER_H
