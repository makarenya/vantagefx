//
// Created by alexx on 28.03.2016.
//

#include "GwtBundle.h"
#include "GwtType.h"
#include <fstream>

namespace vantagefx {
    namespace api {

        void GwtBundle::append(std::shared_ptr<GwtType> type) {
            _types[type->name()] = type;
        }

        std::shared_ptr<GwtType> GwtBundle::type(std::string name) {
            auto it = _types.find(name);
            if (it == _types.end()) return std::shared_ptr<GwtType>();
            return it->second;
        }

        void GwtBundle::printTables(std::string directory) {
            for (auto kvp: _types) {
                if (kvp.second->empty()) continue;
                std::ofstream fs(directory + kvp.first + ".csv");
                kvp.second->printTable(fs);
            }
        }

        GwtVantageFxBundle::GwtVantageFxBundle() {
            add(std::make_shared<GwtComplexType>("com.optionfair.client.model.LutResolverInitPackage", std::vector<GwtField>({
                PtrField("paymentMethodTypes"),
                PtrField("lutTypes"),
                PtrField("paymentMethods"),
                PtrField("assets"),
                PtrField("some_2"),
                PtrField("some_3"),
                PtrField("blackWhiteList"),
                PtrField("someList1"),
                StdField("some_4"),
                StdField("some_5"),
                StringField("some_6"),
                PtrField("some_7"),
                PtrField("counties"),
                StdField("some_8"),
                PtrField("currencies"),
                PtrField("limits"),
                StdField("some_9"),
                PtrField("date"),
                StdField("some_10"),
                StdField("some_11"),
                PtrField("instrumentTypeFilters"),
				PtrField("some_12"),
				PtrField("some_13"),
				PtrField("superRels"),
				StdField("some_14"),
				StdField("some_15"),
				StdField("some_16"),
				StdField("some_17"),
				PtrField("LoginDTO"),
				StdField("some_19"),
				PtrField("markets"),
				PtrField("messages"),
				StdField("some_20"),
				PtrField("servers"),
				PtrField("acceptedMethods"),
				PtrField("pspCcTypeRels"),
				StdField("some_21"),
				PtrField("some_22"),
				PtrField("states"),
				PtrField("some_23"),
				PtrField("trackingInfo"),
				PtrField("verifications"),
				StdField("some_24"),
				StdField("some_25"),
            })));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.TradeLimits", std::vector<GwtField>({
				LongField("some_1"),
				LongField("some_2"),
				LongField("some_3"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.ui.pages.tradePage.spread.High", std::vector<GwtField>({
				StringField("some_1"),
				FloatField("some_2"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.ui.pages.tradePage.spread.Low", std::vector<GwtField>({
				StringField("some_1"),
				FloatField("some_2"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.ui.pages.tradePage.spread.Touch", std::vector<GwtField>({
				StringField("some_1"),
				FloatField("some_2"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.ui.pages.tradePage.spread.NoTouch", std::vector<GwtField>({
				StringField("some_1"),
				FloatField("some_2"),
			})));
			
			add(std::make_shared<GwtComplexType>("com.optionfair.client.ui.pages.tradePage.spread.In", std::vector<GwtField>({
				StringField("some_1"),
				FloatField("some_2"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.ui.pages.tradePage.spread.Out", std::vector<GwtField>({
				StringField("some_1"),
				FloatField("some_2"),
			})));
			
			add(std::make_shared<GwtComplexType>("com.optionfair.client.ui.pages.tradePage.spread.Probability", std::vector<GwtField>({
				StringField("some_1"),
				FloatField("some_2"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.LoginDTO", std::vector<GwtField>({
				PtrField("account")
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.spread.SpreadCalculator", std::vector<GwtField>({
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.Account", std::vector<GwtField>({
				StringField("serialuid"),
				StdField("some_1"),
				StringField("some_2"),
				StdField("some_3"),
				StdField("some_4"),
				LongField("some_5"),
				StdField("some_6"),
				StdField("some_7"),
				StdField("some_8"),
				StdField("some_9"),
				StdField("some_10"),
				LongField("some_11"),
				LongField("some_12"),
				PtrField("some_13"),
				PtrField("some_14"),
				PtrField("some_15"),
				StdField("some_16"),
				StdField("some_17"),
				StdField("some_18"),
				StdField("some_19"),
				StdField("some_20"),
				StdField("some_21"),
				StringField("some_22"),
				StringField("guid"),
				StringField("name"),
				StringField("how"),
				StdField("some_23"),
				StdField("some_24"),
				StringField("email"),
				StdField("some_25"),
				StringField("login"),
				StdField("some_26"),
				StringField("fullName"),
				LongField("some_27"),
				StdField("some_28"),
				LongField("some_29"),
				StdField("some_30"),
				StdField("some_31"),
				StdField("some_32"),
				StdField("some_33"),
				StdField("some_34"),
				StdField("some_35"),
				StdField("some_36"),
				StdField("some_37"),
				StdField("some_38"),
				StdField("some_39"),
				PtrField("some_40"),
				StringField("name"),
				StdField("some_41"),
				StdField("some_42"),
				LongField("some_43"),
				StdField("some_44"),
				LongField("some_45"),
				LongField("some_46"),
				LongField("some_47"),
				StdField("some_48"),
				StdField("some_49"),
				StdField("some_50"),
				StdField("some_51"),
				StdField("some_52"),
				StdField("some_53"),
				StdField("some_54"),
				StdField("some_55"),
				StdField("some_56"),
				PtrField("some_57"),
				LongField("some_58"),
				StdField("some_59"),
				StdField("some_60"),
				StdField("some_61"),
				StdField("some_62"),
				StdField("some_63"),
				StdField("some_64"),
				LongField("some_65"),
				StdField("some_66"),
				StdField("some_67"),
				PtrField("some_68"),
				StringField("answer"),
				StringField("questing"),
				StdField("some_69"),
				StdField("some_70"),
				StdField("some_71"),
				StdField("some_72"),
				FloatField("some_73"),
				PtrField("some_74"),
				LongField("some_75"),
				FloatField("some_76"),
				FloatField("some_77"),
				FloatField("some_78"),
				FloatField("some_79"),
				PtrField("some_80"),
				StdField("some_81"),
				StdField("some_82"),
				FloatField("some_83"),
				FloatField("some_84"),
				StdField("some_85"),
				StdField("some_86"),
				StdField("some_87"),
				StdField("some_88"),
				StdField("some_89"),
				LongField("some_90"),
				StdField("some_91"),
				StdField("some_92"),
				LongField("some_93"),
				StringField("some_94"),
				StringField("some_95"),
				StdField("some_96"),
				StdField("some_97"),
				StdField("some_98"),
				StdField("some_99"),
				FloatField("some_100"),
				StdField("some_101"),
				StdField("some_102"),
				PtrField("some_103"),
				StdField("some_104"),
				StdField("some_105"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.TrackingInfo", std::vector<GwtField>({
				StringField("serialuid"),
				StdField("some_1"),
				LongField("some_2"),
				LongField("some_3"),
				StdField("some_4"),
				StdField("some_5"),
				StdField("some_6"),
				StdField("some_7"),
				StdField("some_8"),
				StdField("some_9"),
				LongField("some_10"),
				PtrField("date"),
			})));

				
			add(std::make_shared<GwtComplexType>("com.optionfair.client.verification.FieldVerification", std::vector<GwtField>({
				StdField("some_1"),
				StdField("some_2"),
				StdField("some_3"),
				StdField("some_4"),
				StdField("some_5"),
				IntField("some_6"),
				StdField("some_7"),
				StringField("name"),
				StdField("some_8"),
				StringField("regex"),
				StdField("some_9"),
				StdField("some_10"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.TrackingInfo", std::vector<GwtField>({
				StringField("serialuid"),
				StdField("some_1"),
				LongField("some_2"),
				LongField("some_3"),
				StdField("some_4"),
				StdField("some_5"),
				StdField("some_6"),
				StdField("some_7"),
				StdField("some_8"),
				StdField("some_9"),
				LongField("some_10"),
				PtrField("date"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.State", std::vector<GwtField>({
				StringField("serialuid"),
				StdField("country"),
				IntField("order"),
				StringField("some_1"),
				StringField("name"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.PspCcTypeRel", std::vector<GwtField>({
				StringField("serialuid"),
				IntField("some_1"),
				LongField("some_2"),
				IntField("some_3"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.MTServer", std::vector<GwtField>({
				StdField("some_1"),
				StdField("some_2"),
				StringField("name"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.SubMarket", std::vector<GwtField>({
				StringField("serialuid"),
				StdField("some_1"),
				IntField("id"),
				IntField("some_2"),
				IntField("some_3"),
				StringField("name"),
				IntField("some_4"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.Market", std::vector<GwtField>({
				StringField("serialuid"),
				IntField("some_1"),
				IntField("some_2"),
				StringField("name"),
				PtrField("subMarkets"),
				StdField("some_3"),
				PtrField("parameters"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.InstrumentTypeFilter", std::vector<GwtField>({
				StringField("serialuid"),
				StdField("some_1"),
				StringField("interval"),
				IntField("order"),
				StdField("some_2"),
				StringField("title"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.InstrumentTypeInstrumentTypeSuperRel", std::vector<GwtField>({
				StringField("serialuid"),
				StdField("some_1"),
				IntField("id"),
				StdField("some_2"),
				StdField("some_3"),
				IntField("order"),
			})));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.model.CurrencyCountryLimit", std::vector<GwtField>({
				StringField("serialuid"),
				StdField("some_1"),
				StdField("some_2"),
				IntField("currencyId"),
				IntField("order"),
				StdField("some_3"),
				LongField("some_4"),
				LongField("some_5"),
				LongField("some_6"),
				LongField("some_7"),
				LongField("some_8"),
				LongField("some_9"),
				LongField("some_10"),
				LongField("some_11"),
				LongField("some_12"),
				LongField("some_13"),
				LongField("some_14"),
				LongField("some_15"),
				LongField("some_16"),
				LongField("some_17"),
				LongField("some_18"),
				LongField("some_19"),
				LongField("some_20"),
				LongField("some_21"),
				LongField("some_22"),
				LongField("some_23"),
				LongField("some_24"),
			})));

            add(std::make_shared<GwtComplexType>("com.optionfair.client.model.Currency", std::vector<GwtField>({
				StringField("serialuid"),
				IntField("id"),
				StringField("name"),
				StringField("symbol"),
				FloatField("some_1"),
				IntField("countryId"),
				StringField("abbr"),
				StdField("some_3"),
				LongField("some_4"),
			})));

            add(std::make_shared<GwtComplexType>("com.optionfair.client.model.Country", std::vector<GwtField>({
                StringField("serialuid"),
                StdField("some_1"),
                StdField("language"),
                IntField("some_2"),
                IntField("some_3"),
                StringField("short_2"),
				StringField("short_3"),
				StringField("fullName"),
                IntField("some_4"),
				StringField("some_5"),
				StringField("some_6"),
            })));

            add(std::make_shared<GwtComplexType>("com.optionfair.client.model.Asset", std::vector<GwtField>({
                StringField("serialuid"),
                FloatField("some_1"),
                FloatField("some_2"),
                FloatField("some_3"),
                StdField("some_4"),
                StdField("some_5"),
                FloatField("some_6"),
                StringField("some_7"),
                StdField("some_8"),
                FloatField("some_9"),
                IntField("some_10"),
                StdField("some_11"),
                IntField("id"),
                FloatField("some_13"),
                IntField("some_14"),
                StdField("some_15"),
                IntField("some_16"),
                FloatField("some_17"),
                StdField("some_18"),
                StdField("some_19"),
                StdField("some_20"),
                IntField("some_21"),
                FloatField("some_22"),
                IntField("some_23"),
                FloatField("some_24"),
                StdField("some_25"),
                StdField("some_26"),
                StdField("some_27"),
                IntField("some_28"),
                IntField("some_29"),
                StdField("some_30"),
				StringField("some_31"),
				StringField("some_32"),
                StringField("some_33"),
                StdField("some_34"),
                FloatField("some_35")
            })));

            add(std::make_shared<GwtComplexType>("com.optionfair.client.model.LutType", std::vector<GwtField>({
                StringField("serialuid"),
                IntField("id"),
                StringField("name"),
                PtrField("luts"),
                StdField(),
                PtrField("parameters")
            })));

            add(std::make_shared<GwtComplexType>("com.optionfair.client.model.Lut", std::vector<GwtField>({
                StringField("serialuid"),
                IntField("id"),
                StringField("name"),
                IntField("lutTypeId")
            })));

            add(std::make_shared<GwtComplexType>("com.optionfair.client.model.PaymentMethod", std::vector<GwtField>({
                StringField("serialuid"),
                IntField("some_1"),
                FloatField("some_2"),
                IntField("id"),
                IntField("some_3"),
                StdField("some_4"),
                IntField("some_5"),
                StringField("some_6"),
                StdField("some_7"),
                StdField("some_8"),
                IntField("some_9"),
                StdField("some_10"),
                IntField("some_11"),
                StdField("some_12")
            })));

			add(std::make_shared<GwtComplexType>("com.optionfair.client.messages.MapMessageSource", std::vector<GwtField>({
				PtrField("value")
			}), 0));
			
			add(std::make_shared<GwtComplexType>("net.sf.gilead.pojo.gwt.collection.ListParameter", std::vector<GwtField>({
                PtrField("value")
            }),0));

            add(std::make_shared<GwtComplexType>("net.sf.gilead.pojo.gwt.collection.MapParameter", std::vector<GwtField>({
                PtrField("value")
            }),0));

            add(std::make_shared<GwtComplexType>("net.sf.gilead.pojo.gwt.basic.IntegerParameter", std::vector<GwtField>({
                PtrField("value")
            }),0));

            add(std::make_shared<GwtComplexType>("net.sf.gilead.pojo.gwt.basic.StringParameter", std::vector<GwtField>({
               StringField("value")
            }),0));

            add(std::make_shared<GwtComplexType>("net.sf.gilead.pojo.gwt.GwtSerializableId", std::vector<GwtField>({
                StringField("entityName"),
                PtrField("id"),
                StringField("stringValue")
            })));


            add(std::make_shared<GwtListType>("java.util.ArrayList"));
			add(std::make_shared<GwtListType>("java.util.LinkedList"));
			add(std::make_shared<GwtListType>("java.util.HashSet"));
            add(std::make_shared<GwtMapType>("java.util.HashMap", 0));
			add(std::make_shared<GwtMapType>("java.util.LinkedHashMap", 1));
			add(std::make_shared<GwtSimpleType>("java.lang.Integer", GwtValueType::Integer));
			add(std::make_shared<GwtSimpleType>("com.optionfair.client.luts.Instrument", GwtValueType::Integer));
            add(std::make_shared<GwtSimpleType>("java.lang.String", GwtValueType::String));
            add(std::make_shared<GwtSimpleType>("java.util.Date", GwtValueType::Date));
        }
    }
}


