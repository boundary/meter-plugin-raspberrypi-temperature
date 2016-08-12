//
// Copyright 2016 BMC Software, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "plugin.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define PLUGIN_METRIC_NAME "RASPBERRY_PI_TEMPERATURE"

#define PLUGIN_PARAM_UNIT "unit"
#define PLUGIN_PARAM_SOURCE "source"
#define PLUGIN_PARAM_INTERVAL "interval"

// Specific data for the Rasperry Pi Temperature
struct pitemp_plugin_data {
    measurement_metric_t metric;
};

// Collector specific data for Raspberry Pi Temperature Collector
struct pitemp_collector_data {
    int unit;
    int interval;
    measurement_metric_t metric;
    measurement_source_t source;
};

typedef struct pitemp_collector_data pitemp_collector_data_t;

/** 
 * Raspberry Pi Temperature  plugin initialization method
 */
plugin_result_t pitemp_plugin_initialize(meter_plugin_t *plugin) {
    plugin_result_t result = PLUGIN_SUCCEED;
    strcpy(plugin->name, "pitemp");
    return result;
}

/**
 * This function collects measurements, in this specific case the Raspberry Pi Temperature
 */
plugin_result_t pitemp_collector_collect(collector_t * collector) {
    plugin_result_t result = PLUGIN_SUCCEED;

    FILE *temperature_file;
    double T;
    temperature_file = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
    if (temperature_file == NULL)
        ; //print some message
    fscanf(temperature_file, "%lf", &T);
    T /= 1000;

    pitemp_collector_data_t *data = collector->data;
    assert(data && "Collector data is NULL");
    measurement_timestamp_t timestamp = time(NULL);
    measurement_value_t value = T;
    collector->send_measurement(data->metric, value, data->source, &timestamp);

    return result;
}

/**
 * Called to initialize each of the collectors
 */
plugin_result_t pitemp_plugin_collector_initialize(meter_plugin_t *plugin, collector_t *collector) {
    plugin_result_t result = PLUGIN_SUCCEED;

    pitemp_collector_data_t * data = malloc(sizeof(pitemp_collector_data_t));
    assert(data);
    memset(data, '\0', sizeof(pitemp_collector_data_t));

    parameter_item_t *item = collector->item;

    // Get the parameters for this collector
    const char * unit = parameter_get_string(item, PLUGIN_PARAM_UNIT);
    data->interval = parameter_get_integer(item, PLUGIN_PARAM_INTERVAL);
    const char * source = parameter_get_string(item, PLUGIN_PARAM_SOURCE);

    strcpy(data->metric, PLUGIN_METRIC_NAME);
    strcpy(data->source, source);

    // Assign the random collector data to the collector
    collector->data = data;

    // Use the source for the name of the collector
    strcpy(collector->name, source);

    // Assign our collector functions
    collector->collect_cb = pitemp_collector_collect;

    return result;
}

/**
 * Plugin main() entry point
 */
int main(int argc, char * argv[]) {
    // Create an instance of a plugin
    meter_plugin_t * plugin = plugin_create();

    // Assign function pointers that get called by the framework
    //    initialize_cb - Initial function called when plugin_run() is called with the meter plugin instance
    //    collector_initialize_cb - Called to initialize the collectors from the plugin parameter items
    plugin->initialize_cb = pitemp_plugin_initialize;
    plugin->collector_initialize_cb = pitemp_plugin_collector_initialize;

    return plugin_run(plugin);
}
