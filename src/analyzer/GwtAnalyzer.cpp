//
// Created by Alexx on 17.04.16.
//

#include "GwtAnalyzer.h"
#include <QDomDocument>
#include <QFile>
#include <boost/filesystem/fstream.hpp>

#include "../api/GwtRequestParser.h"
#include "../api/GwtResponseParser.h"
#include "../api/GwtParser.h"
#include "../api/GwtIterator.h"
#include "FiddlerLogParser.h"
#include "FiddlerLogEntry.h"



namespace vantagefx {
    namespace analyzer {
        using namespace api;
        namespace fs = boost::filesystem;

        GwtAnalyzer::GwtAnalyzer(GwtBundle &bundle)
                : _bundle(bundle) { }


        void GwtAnalyzer::processResponse(fs::path dir, std::string name, std::string content)
        {
            auto data = ParseResponse(content);
            fs::create_directory(dir / "results");
            if (data.version == 7) {
                GwtParser parser(data.strings, data.data, _bundle);
                try {
                    fs::path report = dir / "results" / (name + "_response.xml");
                    auto result = parser.parse();
                    QDomDocument document;
                    auto body = document.createElement("request");
                    result->xml(body);
                    document.appendChild(body);
                    QFile fs(report.string().c_str());
                    fs.open(QIODevice::ReadWrite | QIODevice::Truncate);
                    fs.write(document.toByteArray());
                    fs.close();
                    parser.print(std::cout, 100);
                }
                catch (std::exception &ex) {
                    std::cout << ex.what() << std::endl;
                    parser.print(std::cout, 20);
                }
            }
        }

        GwtObjectPtr GwtAnalyzer::processEntry(fs::path dir, std::string name, FiddlerLogEntry entry)
        {
            fs::create_directory(dir / "results");
            fs::path report = dir / "results" / (name + ".xml");
            QDomDocument document;
            auto body = document.createElement("entry");
            body.setAttribute("method", entry.method().c_str());
            body.setAttribute("url", entry.url().c_str());
            body.setAttribute("code", entry.code().c_str());

            auto data = ParseRequest(entry.request());
            if (data.version == 7) {
                GwtParser parser(data.strings, data.data, _bundle);
                GwtTypePtr type = std::make_shared<GwtRequestType>();
                try {
                    GwtObjectPtr result = std::make_shared<GwtObject>(type);
                    type->parse(parser, result);
                    auto request = document.createElement("request");
                    result->xml(request);
                    body.appendChild(request);
                    parser.print(std::cout, 100);
                }
                catch (std::exception &ex) {
                    std::cout << ex.what() << std::endl;
                    parser.print(std::cout, 20);
                }
            }
            GwtObjectPtr result;
            data = ParseResponse(entry.response());
            if (data.version == 7) {
                GwtParser parser(data.strings, data.data, _bundle);
                try {
                    result = parser.parse();
                    auto response = document.createElement("response");
                    result->xml(response);

                    /*
                    GwtValue search(50);
                    std::vector<std::string> found;
                    result->find(search, found);
                    auto value = result->get("verifications/[Account]");
                    */

                    body.appendChild(response);
                    parser.print(std::cout, 100);
                }
                catch (std::exception &ex) {
                    std::cout << ex.what() << std::endl;
                    parser.print(std::cout, 20);
                }
            }
            document.appendChild(body);
            QFile fs(report.string().c_str());
            fs.open(QIODevice::ReadWrite | QIODevice::Truncate);
            fs.write(document.toByteArray());
            fs.close();
            return result;
        }

        std::vector<FiddlerLogEntry> GwtAnalyzer::parseEntries(fs::path filename)
        {
            std::vector<FiddlerLogEntry> result;
            if (!fs::exists(filename) || !fs::is_regular_file(filename)) return result;
            fs::ifstream file_stream(filename, std::ios::in | std::ios::binary);

            FiddlerLogEntry current;
            FiddlerLogParser parser;

            for (std::istreambuf_iterator<char> it(file_stream); it != std::istreambuf_iterator<char>(); ++it) {
                auto state = parser.advance(*it);
                if (state == FiddlerLogParser::Invalid) {
                    std::cout << "error" << std::endl;
                }
                if (state == FiddlerLogParser::Complete) {
                    result.push_back(parser.result());
                }
            }
            return result;
        }

        std::vector<std::string> GwtAnalyzer::usedBy(GwtObjectPtr object, std::vector<std::string> ids)
        {
            std::vector<std::string> variants;
            std::vector<std::string> old;
            auto first = true;
            for (auto id : ids) {
                GwtValue search(id);
                std::vector<std::string> found;
                for (auto pair : GwtQuery(object, "**/" + id)) {
                    found.push_back(pair.first);
                }
                for (auto pair : GwtQuery(object, "**/[.=" + id + "]")) {
                    found.push_back(pair.first);
                }
                variants.clear();
                for (auto item : found) {
                    std::vector<std::string> parts;
                    boost::split(parts, item, boost::is_any_of("/"));
                    for (auto i = 0; i < parts.size(); i++) {
                        std::string value = "";
                        for (auto j = 0; j < parts.size(); j++) {
                            if (!value.empty()) value += "/";
                            value += (i == j) ? "*" : parts[j];
                        }
                        if (first || std::find(old.begin(), old.end(), value) != old.end()) {
                            if (std::find(variants.begin(), variants.end(), value) == variants.end()) {
                                variants.push_back(value);
                            }
                        }
                    }
                }
                std::swap(variants, old);
                first = false;
            }
            return variants;
        }
    }
}