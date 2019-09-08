#include "Cli.h"

namespace Cli
{

std::string directionToString(Investigation::Direction direction)
{
    if (direction == Investigation::Forward) {
        return "forward";
    }
    if (direction == Investigation::Backward) {
        return "backward";
    }

    return "random";
}

std::string experimentDataToYamlString(const ExperimentData &data)
{
    std::string result;

    for (const Investigation &investigation: data.investigations) {
        result += "investigation:\n  travel_variant: ";
        result += directionToString(investigation.direction) + "\n";
        result += "  experiments:\n  - experiment:\n";

        for (size_t i = 0; i < investigation.experiments.size(); i++) {
            const auto &experiment = investigation.experiments[i];

            result += "      number: " + std::to_string(i) + "\n";
            result += "      input_data:\n        buffer_size: \"" + std::to_string(experiment.bufferSize) + " KiB\"\n";
            result +=
                "      results:\n        duration: \"" + std::to_string(experiment.duration) + " microseconds\"\n";
        }
        result += "\n";
    }

    return result;
}

std::string experimentDataToHtmlString(const ExperimentData &data)
{
    std::string result = R"(<html>
        <head>
          <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
            <script type="text/javascript">
              google.charts.load('current', {'packages':['line']});
              google.charts.setOnLoadCallback(drawChart);

            function drawChart() {

              var data = new google.visualization.DataTable();

              data.addColumn('number', 'X');

        )";

    std::vector<std::string> columnNames;
    std::vector<size_t> bufferSizes;
    std::vector<std::vector<size_t>> rows;

    for (size_t i = 0; i < data.investigations.size(); i++) {
        columnNames.push_back(
            directionToString(data.investigations[i].direction)
        );

        for (size_t e = 0; e < data.investigations[i].experiments.size(); e++) {
            const auto &experiment = data.investigations[i].experiments[e];

            if (i == 0) {
                bufferSizes.push_back(experiment.bufferSize);
                rows.push_back({experiment.duration});
            } else {
                rows[e].push_back(experiment.duration);
            }
        }
    }

    for (const std::string &column : columnNames) {
        result += "data.addColumn('number', '" + column + "');\n";
    }

    result += "\ndata.addRows([\n";
        for (size_t i = 0; i < bufferSizes.size(); i++) {
            result += "[" + std::to_string(bufferSizes[i]) + ", ";

            for (size_t value : rows[i]) {
                result += std::to_string(value) + ", ";
            }

            result += "],\n";
        }
    result += "]);\n";


    result += R"(

              var options = {
                chart: {
                  title: 'CPU cache',
                },
                hAxis: {
                  title: 'Time, microseconds',
                  logScale: true
                },
                vAxis: {
                  title: 'Buffer size, KiB',
                  logScale: false
                },
                width: 900,
                height: 500,
              };

              var chart = new google.charts.Line(document.getElementById('line_top_x'));

              chart.draw(data, google.charts.Line.convertOptions(options));
            }
          </script>
        </head>
        <body>
          <div id="line_top_x"></div>
        </body>
        </html>
        )";


    return result;
}

}