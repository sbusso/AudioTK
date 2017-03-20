/**
 * \file BaseFilter.h
 */

#ifndef ATK_CORE_BASEFILTER_H
#define ATK_CORE_BASEFILTER_H

#include <cstdint>
#include <string>
#include <vector>

#include <ATK/config.h>
#include "config.h"

#if ATK_PROFILING == 1
#include <chrono>
#endif

#if ATK_USE_THREADPOOL == 1
#include <tbb/queuing_mutex.h>
#endif

namespace ATK
{
  /// Base class for all filters
  class BaseFilter
  {
  public:
    BaseFilter(const BaseFilter&) = delete;
    BaseFilter& operator=(const BaseFilter&) = delete;
    /*!
     * @brief Constructor for the core filter
     * @param nb_input_ports is the total number of input ports of this filter
     * @param nb_output_ports is the total number of output ports of this filter
     */
    ATK_CORE_EXPORT BaseFilter(unsigned int nb_input_ports, unsigned int nb_output_ports);
    /// Move constructor
    ATK_CORE_EXPORT BaseFilter(BaseFilter&& other);
    /// Destructor
    virtual ATK_CORE_EXPORT ~BaseFilter();
    
    /*!
     * @brief Links this filter port to another filter's output port
     * @param input_port is the port number where this filter will receive something
     * @param filter is a pointer to the previous filter
     * @param output_port is the port number where this filter will be connected
     */
    ATK_CORE_EXPORT virtual void set_input_port(unsigned int input_port, BaseFilter* filter, unsigned int output_port);
    
    /// Starts processing after calling reset
    ATK_CORE_EXPORT void process(std::size_t size);
    
#if ATK_USE_THREADPOOL == 1
    /// Allows threaded processing
    ATK_CORE_EXPORT void process_parallel(std::size_t size);
#endif

    /*!
     * @brief Setup the input sampling rate, must be in sync with the rest of the pipeline
     * @param rate is the input sampling rate of the plugin
     */
    ATK_CORE_EXPORT void set_input_sampling_rate(std::size_t rate);
    /// Returns this filter internal input sampling rate
    ATK_CORE_EXPORT std::size_t get_input_sampling_rate() const;
    /*!
    * @brief Setup the output sampling rate, must be in sync with the rest of the pipeline
    * @param rate is the output sampling rate of the plugin
    */
    ATK_CORE_EXPORT void set_output_sampling_rate(std::size_t rate);
    /// Returns this filter internal output sampling rate
    ATK_CORE_EXPORT std::size_t get_output_sampling_rate() const;
    
    /// Returns this filter number of input ports
    ATK_CORE_EXPORT std::size_t get_nb_input_ports() const;
    /*!
    * @brief Changes the number of input ports
    * Will trigger a full setup
    * @param nb_ports is the new number of input ports of the plugin
    */
    ATK_CORE_EXPORT virtual void set_nb_input_ports(std::size_t nb_ports);
    /// Returns this filter number of input ports
    ATK_CORE_EXPORT std::size_t get_nb_output_ports() const;
    /*!
    * @brief Changes the number of output ports
    * Will trigger a full setup
    * @param nb_ports is the new number of output ports of the plugin
    */
    ATK_CORE_EXPORT virtual void set_nb_output_ports(std::size_t nb_ports);
    /// Returns this filter input delay (additional pre-0 samples)
    ATK_CORE_EXPORT std::size_t get_input_delay() const;
    /// Returns this filter output delay (additional pre-0 samples)
    ATK_CORE_EXPORT std::size_t get_output_delay() const;
    /*!
    * @brief Changes the filter's latency
    * @param latency is the new lqtency
    */
    ATK_CORE_EXPORT virtual void set_latency(std::size_t latency);
    /// Returns this filter latency
    ATK_CORE_EXPORT std::size_t get_latency() const;
    /// Returns the pipeline global latency from this plugin
    ATK_CORE_EXPORT std::size_t get_global_latency() const;

    /// Resets the filter so that it will process something if needed
    void reset();
    /// Returns the type that the filter processes
    virtual int get_type() const = 0;
    /// Starts processing without calling reset
    void process_conditionnally(std::size_t size);
#if ATK_USE_THREADPOOL == 1
    /// Starts pqrqllel processing without calling reset
    void process_conditionnally_parallel(std::size_t size);
#endif
    /// Resets the internal state of the filter (mandatory before processing a new clip in a DAW for instance)
    ATK_CORE_EXPORT virtual void full_setup();
  
  protected:
    /// The actual filter processing part
    virtual void process_impl(std::size_t size) const = 0;

    /// Prepares the filter by retrieving the inputs arrays
    virtual void prepare_process(std::size_t size) = 0;
    /// Prepares the filter by resizing the outputs arrays
    virtual void prepare_outputs(std::size_t size) = 0;
    
    /// Use this call to recompute internal parameters
    ATK_CORE_EXPORT virtual void setup();

    /// Number of input ports
    std::size_t nb_input_ports;
    /// Number of output ports
    std::size_t nb_output_ports;
    /// Input sampling rate of the plugin
    std::size_t input_sampling_rate;
    /// Output sampling rate of the plugin
    std::size_t output_sampling_rate;
    /// The connections to the output pins of some filters
    std::vector<std::pair<int, BaseFilter*> > connections;

    /// Input delay of the input port
    std::size_t input_delay;
    /// Output delay of the input port
    std::size_t output_delay;
    
    /// Latency of the plugin
    std::size_t latency;
    /// Last processed size
    std::size_t last_size;

  private:
    bool is_reset;
#if ATK_PROFILING == 1
    std::string class_name;
    std::chrono::steady_clock::duration input_conversion_time;
    std::chrono::steady_clock::duration output_conversion_time;
    std::chrono::steady_clock::duration process_time;
#endif
#if ATK_USE_THREADPOOL == 1
    tbb::queuing_mutex mutex;
#endif
  };
}

#endif

