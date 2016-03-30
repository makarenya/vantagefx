//
// Created by alexx on 28.03.2016.
//

#include "GwtBundle.h"
#include <boost/filesystem/fstream.hpp>

namespace vantagefx {
    namespace api {

        namespace fs = boost::filesystem;

        void GwtBundle::append(std::shared_ptr<GwtType> type) {
            _types[type->name()] = type;
        }

        std::shared_ptr<GwtType> GwtBundle::type(std::string name) {
            auto it = _types.find(name);
            if (it == _types.end()) return std::shared_ptr<GwtType>();
            return it->second;
        }

        void GwtBundle::printTables(fs::path directory) {
            for (auto kvp: _types) {
                if (kvp.second->empty()) continue;
                fs::ofstream fs(directory / (kvp.first + ".csv"));
                kvp.second->printTable(fs);
            }
        }

        GwtVantageFxBundle::GwtVantageFxBundle() {

            add("com.optionfair.client.model.ClientUpdatePack", {
                    fstd("some_1"),
                    fptr("assetUpdates"),
                    fptr("some_2"),
                    fstd("some_3"),
                    fstd("some_4"),
                    fstd("some_5"),
                    fstd("some_6"),
                    fstd("some_7"),
                    fptr("options"),
                    fstd("some_10"),
                    fptr("some_11"),
                    fptr("optionUpdates"),
                    fstd("some_12"),
                    fstd("some_13"),
                    fptr("some_14"),
                    fstd("some_15"),
                    fptr("positionsSentimentDto"),
                    fptr("some_16"),
                    fstd("some_17"),
                    fstd("some_18"),
                    fstd("some_19"),
                    fptr("some_20"),
                    flng("some_21"),
                    fptr("some_22"),
                    fstd("some_23"),
                    fstd("some_24")
            });

            add("com.optionfair.client.model.PositionsSentimentDTO", {
                    fptr("map"),
                    fstd("some_1"),
                    fstd("some_2")
            });

            add("com.optionfair.client.model.Option", {
                    fstr("serialuid"),
                    fint("assetId"), // Для какой пары текущий опцион
                    fstd("some_2"),
                    fstd("some_3"),
                    fstd("some_4"),
                    fint("some_5"),
                    fptr("closeDate"),
                    fstd("some_7"),
                    fstd("some_8"),
                    flng("some_9"),
                    fdbl("some_10"),
                    fstd("some_11"),
                    fstd("some_12"),
                    fptr("some_13"),
                    fdbl("some_14"),
                    fdbl("some_15"),
                    flng("some_16"),
                    fint("some_17"),
                    fstd("some_18"),
                    fstd("some_19"),
                    flng("some_20"),
                    fstd("some_21"),
                    fstd("some_22"),
                    fstd("some_23"),
                    fint("return"),
                    fdbl("some_25"),
                    fstd("some_26"),
                    fptr("some_27"),
                    fstd("some_28"),
                    fdbl("some_29"),
                    fdbl("some_30"),
                    fdbl("some_31"),
                    fdbl("some_32"),
                    flng("some_33"),
            });

            add("com.optionfair.client.model.AssetUpdate", {
                    fstr("serialuid"),
                    fint("assetStatusId"), // статус лота (нужен Production)
                    fint("assetId"), // Идентификатор лота
                    fdbl("targetPrice"), // Текущая цена
            });

            add("com.optionfair.client.model.OptionUpdateChangeStrike", {
                    fdbl("some_1"),
                    fdbl("some_2"),
                    flng("some_3"),
                    fdbl("some_4"),
                    fdbl("some_5"),
                    fstd("some_6"),
            });

            add("com.optionfair.client.model.LutResolverInitPackage", {
                    fptr("paymentMethodTypes"),
                    fptr("lutTypes"),
                    fptr("paymentMethods"),
                    fptr("assets"),
                    fptr("some_2"),
                    fptr("some_3"),
                    fptr("blackWhiteList"),
                    fptr("someList1"),
                    fstd("some_4"),
                    fstd("some_5"),
                    fstr("some_6"),
                    fptr("some_7"),
                    fptr("counties"),
                    fstd("some_8"),
                    fptr("currencies"),
                    fptr("limits"),
                    fstd("some_9"),
                    fptr("date"),
                    fstd("some_10"),
                    fstd("some_11"),
                    fptr("instrumentTypeFilters"),
                    fptr("some_12"),
                    fptr("some_13"),
                    fptr("superRels"),
                    fstd("some_14"),
                    fstd("some_15"),
                    fstd("some_16"),
                    fstd("some_17"),
                    fptr("LoginDTO"),
                    fstd("some_19"),
                    fptr("markets"),
                    fptr("messages"),
                    fstd("some_20"),
                    fptr("servers"),
                    fptr("acceptedMethods"),
                    fptr("pspCcTypeRels"),
                    fstd("some_21"),
                    fptr("some_22"),
                    fptr("states"),
                    fptr("some_23"),
                    fptr("trackingInfo"),
                    fptr("verifications"),
                    fstd("some_24"),
                    fstd("some_25")
            });

            add("com.optionfair.client.model.TradeLimits", {
                    flng("some_1"),
                    flng("some_2"),
                    flng("some_3")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.High", {
                    fstr("some_1"),
                    fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.Low", {
                    fstr("some_1"),
                    fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.Touch", {
                    fstr("some_1"),
                    fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.NoTouch", {
                    fstr("some_1"),
                    fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.In", {
                    fstr("some_1"),
                    fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.Out", {
                    fstr("some_1"),
                    fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.Probability", {
                    fstr("some_1"),
                    fdbl("some_2"),
            });

            add("com.optionfair.client.model.LoginDTO", {
                    fptr("account")
            });

            add("com.optionfair.client.spread.SpreadCalculator", {
            });

            add("com.optionfair.client.model.Account", {
                    fstr("serialuid"),
                    fstd("some_1"),
                    fstr("some_2"),
                    fstd("some_3"),
                    fstd("some_4"),
                    flng("some_5"),
                    fstd("some_6"),
                    fstd("some_7"),
                    fstd("some_8"),
                    fstd("some_9"),
                    fstd("some_10"),
                    flng("some_11"),
                    flng("some_12"),
                    fptr("some_13"),
                    fptr("some_14"),
                    fptr("some_15"),
                    fstd("some_16"),
                    fstd("some_17"),
                    fstd("some_18"),
                    fstd("some_19"),
                    fstd("some_20"),
                    fstd("some_21"),
                    fstr("some_22"),
                    fstr("guid"),
                    fstr("name"),
                    fstr("how"),
                    fstd("some_23"),
                    fstd("some_24"),
                    fstr("email"),
                    fstd("some_25"),
                    fstr("login"),
                    fstd("some_26"),
                    fstr("fullName"),
                    flng("some_27"),
                    fstd("some_28"),
                    flng("some_29"),
                    fstd("some_30"),
                    fstd("some_31"),
                    fstd("some_32"),
                    fstd("some_33"),
                    fstd("some_34"),
                    fstd("some_35"),
                    fstd("some_36"),
                    fstd("some_37"),
                    fstd("some_38"),
                    fstd("some_39"),
                    fptr("some_40"),
                    fstr("name2"),
                    fstd("some_41"),
                    fstd("some_42"),
                    flng("some_43"),
                    fstd("some_44"),
                    flng("some_45"),
                    flng("some_46"),
                    flng("some_47"),
                    fstd("some_48"),
                    fstd("some_49"),
                    fstd("some_50"),
                    fstd("some_51"),
                    fstd("some_52"),
                    fstd("some_53"),
                    fstd("some_54"),
                    fstd("some_55"),
                    fstd("some_56"),
                    fptr("some_57"),
                    flng("some_58"),
                    fstd("some_59"),
                    fstd("some_60"),
                    fstd("some_61"),
                    fstd("some_62"),
                    fstd("some_63"),
                    fstd("some_64"),
                    flng("some_65"),
                    fstd("some_66"),
                    fstd("some_67"),
                    fptr("some_68"),
                    fstr("answer"),
                    fstr("questing"),
                    fstd("some_69"),
                    fstd("some_70"),
                    fstd("some_71"),
                    fstd("some_72"),
                    fdbl("some_73"),
                    fptr("some_74"),
                    flng("some_75"),
                    fdbl("some_76"),
                    fdbl("some_77"),
                    fdbl("some_78"),
                    fdbl("some_79"),
                    fptr("some_80"),
                    fstd("some_81"),
                    fstd("some_82"),
                    fdbl("some_83"),
                    fdbl("some_84"),
                    fstd("some_85"),
                    fstd("some_86"),
                    fstd("some_87"),
                    fstd("some_88"),
                    fstd("some_89"),
                    flng("some_90"),
                    fstd("some_91"),
                    fstd("some_92"),
                    flng("some_93"),
                    fstr("some_94"),
                    fstr("some_95"),
                    fstd("some_96"),
                    fstd("some_97"),
                    fstd("some_98"),
                    fstd("some_99"),
                    fdbl("some_100"),
                    fstd("some_101"),
                    fstd("some_102"),
                    fptr("some_103"),
                    fstd("some_104"),
                    fstd("some_105")
            });

            add("com.optionfair.client.model.TrackingInfo", {
                    fstr("serialuid"),
                    fstd("some_1"),
                    flng("some_2"),
                    flng("some_3"),
                    fstd("some_4"),
                    fstd("some_5"),
                    fstd("some_6"),
                    fstd("some_7"),
                    fstd("some_8"),
                    fstd("some_9"),
                    flng("some_10"),
                    fptr("date")
            });


            add("com.optionfair.client.verification.FieldVerification", {
                    fstd("some_1"),
                    fstd("some_2"),
                    fstd("some_3"),
                    fstd("some_4"),
                    fstd("some_5"),
                    fint("some_6"),
                    fstd("some_7"),
                    fstr("name"),
                    fstd("some_8"),
                    fstr("regex"),
                    fstd("some_9"),
                    fstd("some_10")
            });

            add("com.optionfair.client.model.TrackingInfo", {
                    fstr("serialuid"),
                    fstd("some_1"),
                    flng("some_2"),
                    flng("some_3"),
                    fstd("some_4"),
                    fstd("some_5"),
                    fstd("some_6"),
                    fstd("some_7"),
                    fstd("some_8"),
                    fstd("some_9"),
                    flng("some_10"),
                    fptr("date")
            });

            add("com.optionfair.client.model.State", {
                    fstr("serialuid"),
                    fstd("country"),
                    fint("order"),
                    fstr("some_1"),
                    fstr("name")
            });

            add("com.optionfair.client.model.PspCcTypeRel", {
                    fstr("serialuid"),
                    fint("some_1"),
                    flng("some_2"),
                    fint("some_3")
            });

            add("com.optionfair.client.model.MTServer", {
                    fstd("some_1"),
                    fstd("some_2"),
                    fstr("name")
            });

            add("com.optionfair.client.model.SubMarket", {
                    fstr("serialuid"),
                    fstd("some_1"),
                    fint("id"),
                    fint("some_2"),
                    fint("some_3"),
                    fstr("name"),
                    fint("some_4")
            });

            add("com.optionfair.client.model.Market", {
                    fstr("serialuid"),
                    fint("some_1"),
                    fint("some_2"),
                    fstr("name"),
                    fptr("subMarkets"),
                    fstd("some_3"),
                    fptr("parameters")
            });

            add("com.optionfair.client.model.InstrumentTypeFilter", {
                    fstr("serialuid"),
                    fstd("some_1"),
                    fstr("interval"),
                    fint("order"),
                    fstd("some_2"),
                    fstr("title")
            });

            add("com.optionfair.client.model.InstrumentTypeInstrumentTypeSuperRel", {
                    fstr("serialuid"),
                    fstd("some_1"),
                    fint("id"),
                    fstd("some_2"),
                    fstd("some_3"),
                    fint("order")
            });

            add("com.optionfair.client.model.CurrencyCountryLimit", {
                    fstr("serialuid"),
                    fstd("some_1"),
                    fstd("some_2"),
                    fint("currencyId"),
                    fint("order"),
                    fstd("some_3"),
                    flng("some_4"),
                    flng("some_5"),
                    flng("some_6"),
                    flng("some_7"),
                    flng("some_8"),
                    flng("some_9"),
                    flng("some_10"),
                    flng("some_11"),
                    flng("some_12"),
                    flng("some_13"),
                    flng("some_14"),
                    flng("some_15"),
                    flng("some_16"),
                    flng("some_17"),
                    flng("some_18"),
                    flng("some_19"),
                    flng("some_20"),
                    flng("some_21"),
                    flng("some_22"),
                    flng("some_23"),
                    flng("some_24")
            });

            add("com.optionfair.client.model.Currency", {
                    fstr("serialuid"),
                    fint("id"),
                    fstr("name"),
                    fstr("symbol"),
                    fdbl("some_1"),
                    fint("countryId"),
                    fstr("abbr"),
                    fstd("some_3"),
                    flng("some_4")
            });

            add("com.optionfair.client.model.Country", {
                    fstr("serialuid"),
                    fstd("some_1"),
                    fstd("language"),
                    fint("some_2"),
                    fint("some_3"),
                    fstr("short_2"),
                    fstr("short_3"),
                    fstr("fullName"),
                    fint("some_4"),
                    fstr("some_5"),
                    fstr("some_6")
            });

            add("com.optionfair.client.model.Asset", {
                    fstr("serialuid"),
                    fdbl("some_1"),
                    fdbl("some_2"),
                    fdbl("some_3"),
                    fstd("some_4"),
                    fstd("some_5"),
                    fdbl("some_6"),
                    fstr("some_7"),
                    fstd("some_8"),
                    fdbl("some_9"),
                    fint("some_10"),
                    fstd("some_11"),
                    fint("id"),
                    fdbl("some_13"),
                    fint("some_14"),
                    fstd("some_15"),
                    fint("some_16"),
                    fdbl("some_17"),
                    fstd("some_18"),
                    fstd("some_19"),
                    fstd("some_20"),
                    fint("some_21"),
                    fdbl("some_22"),
                    fint("some_23"),
                    fdbl("some_24"),
                    fstd("some_25"),
                    fstd("some_26"),
                    fstd("some_27"),
                    fint("some_28"),
                    fint("some_29"),
                    fstd("some_30"),
                    fstr("some_31"),
                    fstr("some_32"),
                    fstr("some_33"),
                    fstd("some_34"),
                    fdbl("some_35")
            });

            add("com.optionfair.client.model.LutType", {
                    fstr("serialuid"),
                    fint("id"),
                    fstr("name"),
                    fptr("luts"),
                    fstd("some_1"),
                    fptr("parameters")
            });

            add("com.optionfair.client.model.Lut", {
                    fstr("serialuid"),
                    fint("id"),
                    fstr("name"),
                    fint("lutTypeId")
            });

            add("com.optionfair.client.model.PaymentMethod", {
                    fstr("serialuid"),
                    fint("some_1"),
                    fdbl("some_2"),
                    fint("id"),
                    fint("some_3"),
                    fstd("some_4"),
                    fint("some_5"),
                    fstr("some_6"),
                    fstd("some_7"),
                    fstd("some_8"),
                    fint("some_9"),
                    fstd("some_10"),
                    fint("some_11"),
                    fstd("some_12")
            });

            add("com.optionfair.client.messages.MapMessageSource", {
                    fptr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.collection.ListParameter", {
                    fptr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.collection.MapParameter", {
                    fptr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.basic.IntegerParameter", {
                    fptr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.basic.StringParameter", {
                    fstr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.GwtSerializableId", {
                    fstr("entityName"),
                    fptr("id"),
                    fstr("stringValue")
            });


            add(std::make_shared<GwtListType>("java.util.ArrayList"));
            add(std::make_shared<GwtListType>("java.util.LinkedList"));
            add(std::make_shared<GwtListType>("java.util.HashSet"));
            add(std::make_shared<GwtMapType>("java.util.HashMap", 0));
            add(std::make_shared<GwtMapType>("java.util.LinkedHashMap", 1));
            add(std::make_shared<GwtSimpleType>("java.lang.Integer", fint("value")));
            add(std::make_shared<GwtSimpleType>("com.optionfair.client.luts.Instrument", fint("value")));
            add(std::make_shared<GwtSimpleType>("java.lang.Long", flng("value")));
            add(std::make_shared<GwtSimpleType>("java.lang.String", fstr("value")));
            add(std::make_shared<GwtSimpleType>("java.util.Date", fdte("value")));
        }
    }
}


