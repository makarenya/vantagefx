//
// Created by AMakarenya on 26.05.2016.
//

#include "StatisticRenderer.h"

namespace vantagefx {
    namespace controller {
        namespace csv {

            class CsvFileAppender
            {
            public:
                CsvFileAppender(QString fileName, const QList<QString> &header)
                        : _file(fileName),
						  _current(0)
                {
                    _cells = header.count();
                    _file.open(QIODevice::ReadWrite | QIODevice::Append);
                    if (_file.size() == 0) {
                        for(auto item : header) {
                            write(item);
                        }
                    }
                }

                void write(QString value)
                {
                    if (value.indexOf('"') >= 0 || value.indexOf(';') >= 0) {
                        value = "\"" + value.replace("\"", "\"\"") + "\"";
                    }
                    _file.write(value.toUtf8());
                    finalize();
                }

                void write(int value)
                {
                    _file.write(QByteArray::number(value));
                    finalize();
                }

				void write(double value)
				{
					_file.write(QByteArray::number(value));
					finalize();
				}
			
			private:

                void finalize()
                {
                    if (++_current == _cells) {
                        _file.write("\r\n");
						_current = 0;
                    }
                    else {
                        _file.write(";");
                    }
                }

                QFile _file;
                int _current;
                int _cells;
            };

            struct HourCsvFileAppender : public CsvFileAppender
            {
                HourCsvFileAppender(QDir dir, QString name)
                        : CsvFileAppender(path(dir, name), { "Hour", "30s W", "30s F", "30s P", "60s W",
                                                             "60s F", "60s P", "2m W", "2m F", "2m P",
                                                             "5m W", "5m F", "5m P", "Total W", "Total F",
                                                             "Total P" })
                {}

                static QString path(QDir dir, QString name)
                {
                    name.replace("/", "-");
                    return dir.filePath(name + ".csv");
                }
            };

            struct FailsCsvFileAppender : public CsvFileAppender
            {
                FailsCsvFileAppender(QDir dir)
                        : CsvFileAppender(dir.filePath("fails.csv"), { "Date", "Asset", "Sec", "Price", "Line", "Level" })
                {}
            };

            struct VirtualCsvFileAppender : public CsvFileAppender
            {
                VirtualCsvFileAppender(QDir dir)
					: CsvFileAppender(dir.filePath("virtual.csv"), { "Date", "Asset", "Sec", "Up Result", "Down Result", "Call", "Put", "Touch", "NoTouch", "In", "Out", "TouchDown", "NoTouchDown", "Above", "Below", "Sell", "Buy", "UpAsk", "DownAsk", "UpBid", "DownBid" })
				{}
            };
        }

        void HourInfo::write(csv::CsvFileAppender &appender) {
            appender.write(wins);
            appender.write(fails);
            appender.write(draws);
        }

        void OptionInfo::update(int bet, int won) {
            _result += won - bet;
            if (won > bet) {
                _day.wins++;
                _hour.wins++;
                _failsLine = 0;
            }
            else if (won == 0) {
                _day.fails++;
                _hour.fails++;
                _failsLine++;
            }
            else {
                _day.draws++;
                _hour.draws++;
            }
        }

        void OptionInfo::writeHourInfo(csv::CsvFileAppender &appender) {
            _hour.write(appender);
            _hour = HourInfo();
        }

        void OptionInfo::writeDayInfo(csv::CsvFileAppender &appender) {
            _day.write(appender);
            _day = HourInfo();
        }

        void StatisticRenderer::setLogin(const QString &login)
        {
            _login = login;
        }

        void StatisticRenderer::update(QString asset, int index, int64_t bet, int64_t won, int level) {
            auto &info = _assets[asset];
            while(info.size() < 5) {
                info.push_back(OptionInfo());
            }
            auto &opt = info[index];
            if (won > bet && opt.failsLine() >= 5) {
                writeFailsLine(asset, index, opt.failsLine(), opt.result(), level);
				opt.clearResult();
            }
            opt.update(bet, won);
            info[4].update(bet, won);
        }

        void StatisticRenderer::writeHourLine()
        {
			if (_login.isEmpty()) return;
			for (auto it = _assets.begin(); it != _assets.end(); ++it) {
                csv::HourCsvFileAppender file(rootDir(), it.key());

                auto now = QDateTime::currentDateTime().addSecs(-1000);
                file.write(now.toString("yyyy-MM-dd HH"));

                for (auto item : it.value()) {
                    item.writeHourInfo(file);
                }
            }
        }

        void StatisticRenderer::writeDayLine()
        {
			if (_login.isEmpty()) return;
			for (auto it = _assets.begin(); it != _assets.end(); ++it) {
                csv::HourCsvFileAppender file(rootDir(), it.key());

                auto now = QDateTime::currentDateTime().addSecs(-1000);
                file.write(now.toString("yyyy-MM-dd"));

                for (auto item : it.value()) {
                    item.writeDayInfo(file);
                }
            }
        }

        void StatisticRenderer::writeVirtualBets(QMap<QString, QList<viewmodel::VirtualBet>> &bets)
        {
			if (_login.isEmpty()) return;
            csv::VirtualCsvFileAppender file(rootDir());
            for(auto it = bets.begin(); it != bets.end(); ++it) {
                for(auto bet : it.value()) {
                    if (!bet.empty()) {
                        file.write(bet.time().toString("yyyy-MM-dd HH:mm:ss"));
                        file.write(it.key());
                        file.write(bet.seconds());
						if (bet.result() > 0) {
							file.write(70);
							file.write(-100);
						}
						else if (bet.result() < 0) {
							file.write(-100);
							file.write(70);
						}
						else {
							file.write(0);
							file.write(0);
						}
						file.write(bet.rate("Call"));
						file.write(bet.rate("Put"));
						file.write(bet.rate("Touch"));
						file.write(bet.rate("NoTouch"));
						file.write(bet.rate("In"));
						file.write(bet.rate("Out"));
						file.write(bet.rate("TouchDown"));
						file.write(bet.rate("NoTouchDown"));
						file.write(bet.rate("Above"));
						file.write(bet.rate("Below"));
						file.write(bet.rate("Sell"));
						file.write(bet.rate("Buy"));
						file.write(bet.rate("UpAsk"));
						file.write(bet.rate("DownAsk"));
						file.write(bet.rate("UpBid"));
						file.write(bet.rate("DownBid"));
                    }
                }
            }
        }

        void StatisticRenderer::writeFailsLine(QString asset, int index, int fails, int64_t result, int level)
        {
			if (_login.isEmpty()) return;
			csv::FailsCsvFileAppender file(rootDir());
			file.write(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
            file.write(asset);
            switch(index) {
                case 1: file.write("60"); break;
                case 2: file.write("2m"); break;
                case 3: file.write("5m"); break;
                default: file.write("30"); break;
            }
            file.write(QString::number(result / 100.0, '0', 2));
			file.write(QString::number(level));
            file.write(fails);
        }

        QDir StatisticRenderer::rootDir()
        {
            auto docs = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
            auto statistics = QDir(docs.filePath("vantagefx"));
            if (!_login.isEmpty())
                statistics = QDir(statistics.filePath(_login));
            if (!statistics.exists()) statistics.mkpath(statistics.path());
			return statistics;
        }
    }
}

