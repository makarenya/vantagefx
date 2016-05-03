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

        GwtVantageFxBundle::GwtVantageFxBundle() {

            add("com.optionfair.client.model.ClientUpdatePack/839029662", {
                fstd("some_1"),
                fptr("assetUpdates"),
                fptr("money"),
                fptr("some_3"),
                fstd("instrumentType"),
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
                fptr("positionUpdates"),
                fint("some_17"),
                fint("some_18"),
                fint("some_19"),
                fptr("some_20"),
                flng("date"),
                fptr("some_22"),
                fint("some_23"),
                fint("some_24")
            });

            add("com.optionfair.client.model.PositionsSentimentDTO/3338833266", {
                fptr("map"),
                fstd("some_1"),
                fstd("some_2")
            });

            add("com.optionfair.client.model.FeedTickUpdate/4036343884", {
                fdbl("some_1"),
                fptr("date"),
                fstd("some_2"),
                fstd("some_3")
            });

            add("com.optionfair.client.model.Option/887736323", {

                fstr("serialuid"),
                fint("assetId"), // Для какой пары текущий опцион
                fstd("some_2"),
                fint("optionSeconds"),
                fstd("some_4"),
				fint("some_5"),
				fptr("closeDate"),
                fptr("some_7"),
                fint("expiryTypeId"),
                flng("id"),
                fdbl("some_10"),
                fint("instrumentId"),
                fstd("some_12"),
                fptr("some_13"),
                fdbl("some_14"),
                fdbl("some_15"),
                flng("some_16"),
                fint("some_17"),
                fint("some_18"),
                fint("some_19"),
                flng("some_20"),
				fstd("some_21"),
				fstd("some_22"),
				fint("some_23"),
				fint("return"),
				fstr("some_24"),
                fdbl("some_25"),
                fstd("some_26"),
                fptr("openFrom"),
                fint("optionStatus"),
                fdbl("some_29"),
                fdbl("some_30"),
                fdbl("some_31"),
                fdbl("some_32"),
                flng("some_33"),
            });

            add("com.optionfair.client.model.PositionUpdate/3887973789", {
                fdbl("closedPrice"),//0.000000e+00
                fptr("positionCloseDate"),//0
                flng("transactionId"),//KlvU
                fint("instrumentId"),//311
                flng("date"),//VRdvf30
                flng("returned"),//A
                flng("some_7"),//A
                fint("positionStatus"),//18
                fdbl("some_9"),//0.000000e+00
            });

            add("com.optionfair.client.model.AssetUpdate/3263927368", {
                fstr("serialuid"),
                fint("assetStatusId"), // статус лота (нужен Production)
                fint("assetId"), // Идентификатор лота
                fdbl("targetPrice"), // Текущая цена
            });

            add("com.optionfair.client.model.OptionUpdateChangeStrike/3365438592", {
                fdbl("targetPrice"),
                fdbl("some_2"),
                flng("optionId"),
                fdbl("some_4"),
                fdbl("some_5"),
                fint("optionStatus"),
            });

			add("com.optionfair.client.resolver.BrandCurrencyCountryResolver/4149870428", {
                fptr("countries"),
                fptr("currencies"),
                fptr("currencyMap"),
                fstd("some_1"),
			});

            add("com.optionfair.client.model.LutResolverInitPackage/3212179674", {
                fptr("pspToPspType"),
                fptr("lutTypes"),
                fptr("paymentMethods"),
                fptr("assets"),
                fptr("someToPspCcTypeRels"),
                fptr("some_3"),
                fptr("blackWhiteList"),
                fptr("someList1"),
                fptr("brandCurrencyCountryResolver"),
                fstd("externalId"),
                fstd("some_5"),
                fstr("enfinium"),
                fptr("breandPspRels"),
                fptr("some_8"),
                fptr("brandRegulations"),
                fptr("primaryCurrencies"),
                fptr("limits"),
                fstd("basic"),
                fptr("date"),
                fptr("primaryDateFormat"),
                fstd("some_10"),
                fstd("some_11"),
                fptr("instrumentTypeFilters"),
                fptr("instrumentTypeSuperToInstrumentTypes"),
                fptr("instrumentTypeToInstrument"),
                fptr("superRels"),
                fstd("some_15"),
                fstd("some_16"),
                fstd("num_99999"),
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

            add("com.optionfair.client.model.TradeLimits/516060049", {
                flng("some_1"),
                flng("some_2"),
                flng("some_3")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.High/1043352957", {
                fstr("some_1"),
                fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.Low/1807028581", {
                fstr("some_1"),
                fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.Touch/1022019549", {
                fstr("some_1"),
                fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.NoTouch/1845245280", {
                fstr("some_1"),
                fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.In/3813998762", {
                fstr("some_1"),
                fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.Out/2494560053", {
                fstr("some_1"),
                fdbl("some_2")
            });

            add("com.optionfair.client.ui.pages.tradePage.spread.Probability/1287790398", {
                fstr("some_1"),
                fdbl("some_2"),
            });

            add("com.optionfair.client.model.LoginDTO/3023568058", {
                fptr("account"),
                fstr("login"),
                fstr("some_1"),
                fptr("someMap"),
                fstd("some_2"),
                fstd("some_3"),
                fdbl("some_4"),
                fint("some_5"),
                fstd("some_6"),
                fptr("tradeLimits"),
                fstd("some_8"),
                fstd("some_9")
            });

            add("com.optionfair.client.spread.SpreadCalculator/3758543021", {
                flng("some_1"),
                fdbl("some_2"),
                fdbl("some_3"),
                fdbl("some_4"),
                fdbl("some_5"),
                fptr("spreads"),
                fstd("some_6"),
                fstd("some_7"),
                fdbl("some_8"),
                fdbl("some_9"),
                fstd("some_10"),
                fint("some_11"),
                fstd("some_12"),
                fstd("some_13"),
                fstd("some_14"),
                flng("some_15"),
                fstd("some_16"),
            });



            add("com.optionfair.client.model.Account/2810493929", {
                fstr("serialuid"),
                fstd("some_1"),
                fstr("regular"),
                fint("some_2"),
                fstd("some_3"),
                flng("some_4"),
                fstd("some_5"),
                fstd("some_6"),
                fstd("some_7"),
                fstd("some_8"),
                fstd("some_9"),
                flng("some_10"),
                flng("some_11"),
                fptr("some_date"),
                fptr("someList_1"),
                fptr("someList_2"),
                fdbl("some_12"),
                fint("externalId"),
                fptr("integer_1"),
                fstd("some_13"),
                fstd("some_14"),
                fint("some_15"),
                fstr("gotham"),
                fstr("guid"),
                fstr("name"),
                fstr("how"),
                fstd("some_16"),
                fstd("some_17"),
                fstr("email"),
                fstd("some_18"),
                fstr("login"),
                fstd("some_19"),
                fstr("fullName"),
                flng("some_20"),
                fstd("some_21"),
                flng("accountId"),
                fstd("some_23"),
                fstd("some_24"),
                fstd("some_25"),
                fstd("some_26"),
                fstd("some_27"),
                fstd("some_28"),
                fstd("some_29"),
                fstd("some_30"),
                fstd("some_31"),
                fstd("some_32"),
                fptr("lastVisit"),
                fstr("shortName"),
                fstd("some_33"),
                fstd("some_34"),
                flng("some_35"),
                fstd("some_36"),
                flng("some_37"),
                flng("some_38"),
                flng("some_39"),
                fstd("some_40"),
                fstd("some_41"),
                fstd("some_42"),
                fstd("some_43"),
                fstd("some_44"),
                fstd("some_45"),
                fstd("some_46"),
                fstr("some_47"),
                fstd("some_48"),
                fptr("joinDate"),
                flng("some_49"),
                fint("some_50"),
                fptr("integer_2"),
                fstd("some_51"),
                fint("some_52"),
                fint("some_53"),
                flng("some_54"),
                fstd("some_55"),
                fstd("some_56"),
                fptr("some_57"),
                fstr("answer"),
                fstr("questing"),
                fstd("some_58"),
                fint("some_59"),
                fint("some_60"),
                fstr("some_61"),
                fdbl("some_62"),
                fptr("spreadCalculator"),
                fstd("some_63"),
                fstd("some_64"),
                flng("some_65"),
            });

            add("com.optionfair.client.model.TrackingInfo/2864454600", {
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


            add("com.optionfair.client.verification.FieldVerification/2630919542", {
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

            add("com.optionfair.client.verification.FieldVerification/2808143106", {
                fstd("some_1"),
                fstr("some_2"),
                fstd("some_3"),
                fstd("some_4"),
                fstd("some_5"),
                fint("some_6"),
                fstd("some_7"),
                fstd("some_8"),
                fstr("name"),
                fstd("some_9"),
                fstr("regex"),
                fstd("some_10"),
                fstd("some_11"),
                fstd("some_12")
            });

            add("com.optionfair.client.model.State/1216850312", {
                fstr("serialuid"),
                fstd("country"),
                fint("order"),
                fstr("some_1"),
                fstr("name")
            });

            add("com.optionfair.client.model.PspCcTypeRel/300072509", {
                fstr("serialuid"),
                fint("creditCardTypeId"),
                flng("id"),
                fint("paymentMethodTypeId")
            });

            add("com.optionfair.client.model.MTServer/2069697836", {
                fstd("some_1"),
                fstd("some_2"),
                fstr("name")
            });

            add("com.optionfair.client.model.SubMarket/2699240068", {
                fstr("serialuid"),
                fstd("some_1"),
                fint("id"),
                fint("some_2"),
                fint("some_3"),
                fstr("name"),
                fint("some_4")
            });

            add("com.optionfair.client.model.Market/3553066736", {
                fstr("serialuid"),
                fint("id"),
                fint("some_2"),
                fstr("name"),
                fptr("subMarkets"),
                fstd("some_3"),
                fptr("parameters")
            });

            add("com.optionfair.client.model.InstrumentTypeFilter/1437805786", {
                fstr("serialuid"),
                fstd("some_1"),
                fstr("interval"),
                fint("order"),
                fstd("some_2"),
                fstr("title")
            });

            add("com.optionfair.client.model.InstrumentTypeInstrumentTypeSuperRel/1928340203", {
                fstr("serialuid"),
                fint("some_1"),
                fint("id"),
                fint("instrumentTypeId"),
                fint("instrumentTypeSuperId"),
                fint("order")
            });

            add("com.optionfair.client.model.CurrencyCountryLimit/94057825", {
                fstr("serialuid"),
                fstd("externalId"),
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
                flng("some_24"),
				flng("some_25"),
				flng("some_26")
			});

            add("com.optionfair.client.model.Currency/2484116626", {
                fstr("serialuid"),
                fint("id"),
                fstr("name"),
                fstr("symbol"),
                fstd("some_1"),
                fint("countryId"),
                fdbl("some_3"),
                fstd("some_4"),
                fstr("some_5"),
                fstr("abbr"),
                fstd("some_6"),
                flng("some_7")
            });

            add("com.optionfair.client.model.BreandPspRel/2784143874", {
                fstr("serialuid"),
                fint("externalId"),
                fstd("some_1"),
                fstd("some_2"),
                fstd("some_3"),
                fstd("some_4"),
                fstd("some_5"),
                fstd("some_6"),
                fstd("some_7"),
                fstd("some_8"),
                fstd("some_9"),
                fstd("some_10"),
                fstd("some_11")
            });

            add("com.optionfair.client.model.BrandRegulation/895814788", {
                fstr("serialuid"),
                fint("externalId"),
                fstd("some_1"),
                fstd("some_2"),
                fstd("some_3"),
                fstd("some_4"),
                fstd("some_5"),
                fstd("some_6"),
                fstd("some_7")
            });

			add("com.optionfair.client.model.DateTimeFormatOF/3059496483", {
				fstr("serialuid"),
				fstr("full_date"),
				fstr("mid_date"),
				fstr("short_date"),
				fstr("very_short"),
				fstd("some_1"),
				fstr("full_time"),
				fstr("mid_time"),
				fstr("short_time")
			});

            add("com.optionfair.client.model.Country/2926175992", {
                fstr("serialuid"),
                fptr("dateTimeFormat"),
                fstd("some_2"),
                fstd("language"),
                fint("some_3"),
                fint("some_4"),
                fstr("short_2"),
                fstr("short_3"),
                fstr("fullName"),
                fint("some_5"),
                fstr("some_6"),
                fstr("some_7")
            });

            add("com.optionfair.client.model.Asset/3083867617", {
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
				fint("some_11"),
				fint("id"),
				fdbl("some_13"),
				fint("some_14"),
				fint("some_15"),
				fptr("some_16"),
				fint("some_17"),
				fdbl("some_18"),
				fstd("some_19"),
				fstd("some_20"),
				fstd("some_21"),
				fint("some_22"),
				fint("some_23"),
				fint("some_24"),
				fdbl("some_25"),
				fint("some_26"),
				fdbl("some_27"),
				fstd("some_28"),
				fint("some_29"),
				fint("some_30"),
				fstd("some_31"),
				fint("subMarketId"),
				fstd("some_33"),
				fstr("name"),
				fstr("some_35"),
				fstr("some_36"),
				fstd("some_37"),
				fdbl("some_38")
			});

            add("com.optionfair.client.model.LutType/4271068468", {
                fstr("serialuid"),
                fint("id"),
                fstr("name"),
                fptr("luts"),
                fstd("some_1"),
                fptr("parameters")
            }, "name");

            add("com.optionfair.client.model.Lut/3629739227", {
                fstr("serialuid"),
                fint("id"),
                fstr("name"),
                fint("lutTypeId")
            }, "name");

			add("com.optionfair.client.model.PaymentMethod/4059381229", {
				fstr("serialuid"),//6 [rO0ABXcEAAAAAA==]
				fint("commissionType"),//128 [UpBid]
				fdbl("some_2"),//3.000000e+02
				fint("id"),//93 [Tax]
				fint("some_4"),//1 [com.optionfair.client.model.LutResolverInitPackage/3212179674]
				fstd("some_5"),//0
				fint("some_6"),//8 [com.optionfair.client.model.Lut/3629739227]
				fstr("some_7"),//204 [Wire]
				fstd("some_8"),//0
				fstd("some_9"),//0
				fint("some_10"),//1 [com.optionfair.client.model.LutResolverInitPackage/3212179674]
				fint("regulationId"),//1502
				fstd("some_12"),//0
				fstd("some_13"),//267 [IdPay_cysec]
			});
			
			add("com.optionfair.client.model.TradeActionResponse/3233850074", {
                flng("accountId"), //ISw
                fint("some_2"),//0
                flng("money"),//Po
                fint("some_4"),//4
                fdbl("some_5", true),//0
                fdbl("some_6"),//1.13335
                fint("some_7"),//0
                fstr("some_8"),//2 [ACR+yRO4lkJ3cjauwhbeAQ==]
                fdbl("some_9", true),//0
                fint("some_10"),//1 [com.optionfair.client.model.TradeActionResponse/3233850074]
                flng("some_11"),//Xm$s
                fint("positionType"),//49
                flng("some_13"),//A
                fstr("some_14"),//3 [0]
                fdbl("some_15"),//1.13335
                fstr("some_16"),//4 [1.13335]
                fint("some_17"),//0
                flng("purchasedAt"),//VRdvfvC
                fint("some_19")
            });

            add("com.optionfair.client.model.Position/1677341959", {
                fstr("serialuid"),//2 [rO0ABXcEAAAAAA==]
                flng("accountId"),//ISw
                fint("some_2"),//0
                flng("some_3"),//Po
                flng("some_4"),//A
                fdbl("some_5"),//0.000000e+00
                fint("some_6"),//0
                fptr("some_7"),//0
                fdbl("some_8"),//0.000000e+00
                fdbl("some_9"),//0.000000e+00
                fptr("expiryDate"),//3 [java.util.Date/3385151746]
                flng("some_10"),//A
                flng("transactionId"),//KlvU
                flng("some_12"),//Po
                fint("some_13"),//1 [com.optionfair.client.model.Position/1677341959]
                fint("some_14"),//0
                fptr("openDate"),//3 [java.util.Date/3385151746]
                fstr("some_15"),//0
                flng("some_16"),//L2
                flng("some_17"),//L2
                flng("some_18"),//A
                flng("some_19"),//A
                fint("some_20"),//1 [com.optionfair.client.model.Position/1677341959]
                fint("positionOpenType"),//264
                fptr("option"),
                fptr("some_date_3"),
                fint("some_22"),
                fint("positionType"),//49
                flng("some_24"),//A
                flng("some_25"),//A
                fint("some_26"),//0
                fint("some_27"),//0
                flng("some_28"),//A
                flng("some_29"),//A
                fdbl("some_30"),//1.133350e+00
                fint("positionStatus"),//18
                fdbl("some_32"),//0.000000e+00
                fdbl("some_33"),//1.133350e+00
                fdbl("some_34"),//0.000000e+00
                fdbl("some_35"),//0.000000e+00
                fdbl("some_36"),//1.133350e+00
                fint("some_37"),//0
                fdbl("some_38"),//0.000000e+00
                flng("some_39"),//U7fV
                fdbl("some_40"),//0.000000e+00
            });

            add("com.optionfair.client.messages.MapMessageSource/3864621178", {
                    fptr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.collection.ListParameter/3890945144", {
                    fptr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.collection.MapParameter/1940144086", {
                    fptr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.basic.IntegerParameter/3421404974", {
                    fptr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.basic.StringParameter/2783524083", {
                    fstr("value")
            }, "value");

            add("net.sf.gilead.pojo.gwt.GwtSerializableId/2844303195", {
                    fstr("entityName"),
                    fptr("id"),
                    fstr("stringValue")
            });


            add(std::make_shared<GwtListType>("java.util.ArrayList/4159755760"));
            add(std::make_shared<GwtListType>("java.util.LinkedList/3953877921"));
            add(std::make_shared<GwtListType>("java.util.HashSet/3273092938"));
            add(std::make_shared<GwtMapType>("java.util.HashMap/1797211028", 0));
            add(std::make_shared<GwtMapType>("java.util.LinkedHashMap/3008245022", 1));
			add("java.lang.Integer/3438268394", { fint("value") }, "value");
			add("com.optionfair.client.luts.Instrument/3175034088", { fint("value") }, "value");
			add("java.lang.Long/4227064769", { flng("value") }, "value");
			add("java.lang.String/2004016611", { fstr("value") }, "value");
			add("java.util.Date/3385151746", { fdte("value") }, "value");
            add("java.lang.Double/858496421", { fdbl("value") }, "value");
        }
    }
}


