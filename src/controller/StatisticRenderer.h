//
// Created by AMakarenya on 26.05.2016.
//

#ifndef VANTAGEFX_STATISTICRENDERER_H
#define VANTAGEFX_STATISTICRENDERER_H

#include <QtCore>
#include "../model/TransactionModel.h"


namespace vantagefx {
    namespace controller {
        namespace csv {
                class CsvFileAppender;
        };

        struct HourInfo
        {
            int wins;
            int fails;
            int draws;

            HourInfo()
                    : wins(0),
                      fails(0),
                      draws(0) {}

            void write(csv::CsvFileAppender &appender);
        };

        class OptionInfo {
        public:
            void update(int bet, int won);
            void writeHourInfo(csv::CsvFileAppender &appender);
            void writeDayInfo(csv::CsvFileAppender &appender);

            int failsLine() const { return _failsLine; }
            int64_t result() const { return _result; }
			void clearResult() { _result = 0; }

        private:
            HourInfo _hour;
            HourInfo _day;
            int64_t _result;
            int _failsLine;
        };

        class StatisticRenderer {
        public:
            void update(QString asset, int index, int64_t bet, int64_t won);
            void writeHourLine();
            void writeDayLine();

        private:
            void writeFailsLine(QString asset, int index, int fails, int64_t result);
            QDir rootDir();
            QMap<QString, QList<OptionInfo>> _assets;
        };
    }
}


#endif //VANTAGEFX_STATISTICRENDERER_H
