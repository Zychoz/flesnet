/**
 * @file
 * @author Dirk Hutter <hutter@compeng.uni-frankfurt.de>
 *
 */
#include "httpClient.hpp"
#include "flib.h"
#include "device_operator.hpp"
#include <iostream>
#include <sys/time.h>
#include <unistd.h>


std::ostream& operator<<(std::ostream& os, flib::flib_link::data_sel_t sel) {
  switch (sel) {
  case flib::flib_link::rx_disable:
    os << "disable";
    break;
  case flib::flib_link::rx_emu:
    os << "    emu";
    break;
  case flib::flib_link::rx_link:
    os << "   link";
    break;
  case flib::flib_link::rx_pgen:
    os << "   pgen";
    break;
  default:
    os.setstate(std::ios_base::failbit);
  }
  return os;
}



int main(int argc, char* argv[]) {
    
  HttpClient http("http://localhost:8086");

    

    
    
    
  try {

    // display help if any parameter given
    if (argc != 1) {
      (void)argv;
      std::cout
          << "Displays status and performance counters for all FLIB links.\n"
             "Per FLIB counters:\n"
             "idle:     PCIe interface is idle (ratio)\n"
             "stall:    back pressure on PCIe interface from host (ratio)\n"
             "trans:    data is transmitted via PCIe interface (ratio)\n"
             "Per link status/counters:\n"
             "link:     flib/link\n"
             "data_sel: choosen data source\n"
             "up:       flim channel_up\n"
             "he:       aurora hard_error\n"
             "se:       aurora soft_error\n"
             "eo:       eoe fifo overflow\n"
             "do:       data fifo overflow\n"
             "d_max:    maximum number of words in d_fifo\n"
             "dma_s:    stall from dma mux (ratio)\n"
             "data_s:   stall from full data buffer (ratio)\n"
             "desc_s:   stall from full desc buffer (ratio)\n"
             "bp:       back pressure to link (ratio)\n"
             "rate:     ms processing rate (Hz*)\n"
             "* Based on the assumption that the PCIe clock is exactly 100 "
             "MHz.\n"
             "  This may not be true in case of PCIe spread-spectrum "
             "clocking.\n";
      std::cout << std::endl;
    }

    std::unique_ptr<pda::device_operator> dev_op(new pda::device_operator);
    std::vector<std::unique_ptr<flib::flib_device_flesin>> flibs;
    uint64_t num_dev = dev_op->device_count();

    int dev_counter;
    for (size_t i = 0; i < num_dev; ++i) {
      flibs.push_back(std::unique_ptr<flib::flib_device_flesin>(
          new flib::flib_device_flesin(i)));
        ++dev_counter;
        
    }
    
    
    size_t j = 0;
    string device_info;
    std::stringstream ss;
    string test_request;
    j = 0;
      
      
    /*
    for (auto& flib : flibs) {
      float pci_stall = flib->get_pci_stall();
      float pci_trans = flib->get_pci_trans();
      float pci_idle = 1 - pci_trans - pci_stall;
      dev_info_ss[j] << "FLIB_" << j << "_(" << flib->print_devinfo() << "),";
      dev_info_ss[j] << std::setprecision(4) << "PCIe_idle=" << pci_idle << ",stall=" << pci_stall
        << ",trans=" << pci_trans;
      ++j;
    }
    device_info = dev_info_ss.str();
    std::cout << std::endl;

    
    std::cout
        << "link  data_sel  up  he  se  "
           "eo  do  d_max     dma_s    data_s    desc_s        bp     rate\n";
    */
      
    
    for (auto& flib : flibs) {
      size_t num_links = flib->number_of_hw_links();
      std::vector<flib::flib_link_flesin*> links = flib->links();

        
      float pci_stall = flib->get_pci_stall();
      float pci_trans = flib->get_pci_trans();
      float pci_idle = 1 - pci_trans - pci_stall;
      
        
       
     /*
      if (j != 0) {
        std::cout << "\n";
      }
      */
        
        
      
      for (size_t i = 0; i < num_links; ++i) {
        flib::flib_link_flesin::link_status_t status =
            links.at(i)->link_status();
        flib::flib_link_flesin::link_perf_t perf = links.at(i)->link_perf();
        
        struct timeval timestamp;
        gettimeofday(&timestamp, NULL);
        long int ms = timestamp.tv_sec * 1000 + timestamp.tv_usec / 1000;
        
          
        unsigned int microseconds = 1000;
          
        usleep(microseconds);
          
        /*
        ss << std::setw(2) << j << "/" << i << "  ";
        ss << std::setw(8) << links.at(i)->data_sel() << "  ";
        // status
        ss << std::setw(2) << status.channel_up << "  ";
        ss << std::setw(2) << status.hard_err << "  ";
        ss << std::setw(2) << status.soft_err << "  ";
        ss << std::setw(2) << status.eoe_fifo_overflow << "  ";
        ss << std::setw(2) << status.d_fifo_overflow << "  ";
        ss << std::setw(5) << status.d_fifo_max_words << "  ";
        // perf counters
        ss << std::setprecision(3); // percision + 5 = width
        ss << std::setw(8) << perf.dma_stall << "  ";
        ss << std::setw(8) << perf.data_buf_stall << "  ";
        ss << std::setw(8) << perf.desc_buf_stall << "  ";
        ss << std::setw(8) << perf.din_full << "  ";
        ss << std::setprecision(7) << std::setw(7) << perf.event_rate << "  ";
        ss << std::endl;
        */
        
        //Measurement
        ss << "Flib_Status,";
        //Tags
        ss << "FLIB=\"FLIB_" << j << "_(" << flib->print_devinfo() << ")\",";
        ss << "data_sel=\"" << links.at(i)->data_sel() << "\",";
        ss << "up=\"" << status.channel_up << "\" ";
        //Fields
        ss << std::setprecision(4) << "PCIe_idle=\"" << pci_idle << "\",stall=\"" << pci_stall
        << "\",trans=\"" << pci_trans << "\"";
        ss << device_info;
        ss << ",link=\"" << i << "\",";
        ss << "he=\"" << status.hard_err << "\",";
        ss << "se=\"" << status.soft_err << "\",";
        ss << "eo=\"" << status.eoe_fifo_overflow << "\",";
        ss << "do=\"" << status.d_fifo_overflow << "\",";
        ss << "d_max=\"" << status.d_fifo_max_words << "\",";
        // perf counters
        ss << std::setprecision(3); // percision + 5 = width
        ss << "dma_s=\"" << perf.dma_stall << "\",";
        ss << "data_s=\"" << perf.data_buf_stall << "\",";
        ss << "desc_s=\"" << perf.desc_buf_stall << "\",";
        ss << "bp=\"" << perf.din_full << "\",";
        ss << "rate=\"" << std::setprecision(7) << perf.event_rate << "\" ";
        //Timestamp
        ss << ms;
        ss << std::endl;
        test_request = test_request + ss.str();
        ss.str("");
        
        
      }
      
      // std::cout << ss.str();
      ++j;
    }
    //std::cout << test_request << std::endl;
    http.putreq("/write?","db=mydb",test_request,"POST");
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
