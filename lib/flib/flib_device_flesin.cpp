/**
 * @file
 * @author Dirk Hutter <hutter@compeng.uni-frankfurt.de>
 *
 */

#include "flib_device_flesin.hpp"
#include "flib_link_flesin.hpp"

namespace flib {

flib_device_flesin::flib_device_flesin(int device_nr) : flib_device(device_nr) {
  init();
  m_reg_perf_interval_cached = m_register_file->get_reg(RORC_REG_SYS_PERF_INT);
}

flib_device_flesin::flib_device_flesin(uint8_t bus,
                                       uint8_t device,
                                       uint8_t function)
    : flib_device(bus, device, function) {
  init();
}

void flib_device_flesin::init() {
  check_hw_ver(hw_ver_table_flesin);
  // create link objects
  uint8_t num_links = number_of_hw_links();
  for (size_t i = 0; i < num_links; i++) {
    m_link.push_back(std::unique_ptr<flib_link>(
        new flib_link_flesin(i, m_device.get(), m_bar.get())));
  }
}

std::vector<flib_link_flesin*> flib_device_flesin::links() {
  std::vector<flib_link_flesin*> links;
  for (auto& l : m_link) {
    links.push_back(static_cast<flib_link_flesin*>(l.get()));
  }
  return links;
}

flib_link_flesin* flib_device_flesin::link(size_t n) {
  return static_cast<flib_link_flesin*>(m_link.at(n).get());
}

void flib_device_flesin::id_led(bool enable) {
  m_register_file->set_bit(RORC_REG_APP_CFG, 0, enable);
}

//////*** Performance Counters ***//////

// set messurement avaraging interval in ms (max 17s)
void flib_device_flesin::set_perf_interval(uint32_t interval) {
  if (interval > 17000) {
    interval = 17000;
  }
  m_reg_perf_interval_cached = interval * (pci_clk * 1E-3);
  m_register_file->set_reg(RORC_REG_SYS_PERF_INT, m_reg_perf_interval_cached);
}

// back pressure from pcie core (ratio)
float flib_device_flesin::get_pci_stall() {
  float pci_nrdy =
      static_cast<float>(m_register_file->get_reg(RORC_REG_PERF_PCI_NRDY));
  return pci_nrdy / m_reg_perf_interval_cached;
}

// words accepted from pcie core (ratio)
float flib_device_flesin::get_pci_trans() {
  float pci_trans =
      static_cast<float>(m_register_file->get_reg(RORC_REG_PERF_PCI_TRANS));
  return pci_trans / m_reg_perf_interval_cached;
}
}
