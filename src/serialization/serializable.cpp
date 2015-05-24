#include "serialization/serializable.hh"
#include "serialization/archive.hh"
#include "serialization/serial.hpp"

#include <iostream>

namespace serialization {

  std::map< std::string, std::function<Serializable*(const Archive&)> >&
				Serializable::getAllocationMap() {
    static std::map< std::string, std::function<Serializable*(const Archive&)> > _intern;

    return (_intern);
  }

  std::map< std::string, std::string >&
				Serializable::getTypeIdMap() {
    static std::map< std::string, std::string > _intern;

    return (_intern);
  }

  Serializable*			Serializable::Instantiate(const Archive& archive) {
    if (getAllocationMap().end() == getAllocationMap().find(archive["type"].as<std::string>()))
      throw error::undefined_type_reference(archive["type"].as<std::string>());
    try {
      return getAllocationMap() .at(archive["type"].as<std::string>()) (archive["object"].as<const Archive&>());
    } catch (const error::undefined_type_reference& e) {
      throw error::backtrace_serial_error(archive["type"].as<std::string>(), e.what());
    } catch (const error::undefined_variable_reference& e) {
      throw error::backtrace_serial_error(archive["type"].as<std::string>(), e.what());
    } catch (const error::backtrace_serial_error& e) {
      throw error::backtrace_serial_error(archive["type"].as<std::string>(), e.what());
    } catch (const std::out_of_range&) {
      throw error::undefined_type_reference(archive["type"].as<std::string>());
    }
  }

  Serializable*			Serializable::Instantiate(const Serial& serial) {
    const Archive&		archive = serial.as<const Archive&>();

    try {
      return getAllocationMap() .at(archive["type"].as<std::string>()) (archive["object"].as<const Archive&>());
    } catch (const error::incorrect_object& e) {
      throw error::backtrace_serial_error(archive["type"].as<std::string>(), e.what());
    } catch (const error::undefined_type_reference& e) {
      throw error::backtrace_serial_error(archive["type"].as<std::string>(), e.what());
    } catch (const error::undefined_variable_reference& e) {
      throw error::backtrace_serial_error(archive["type"].as<std::string>(), e.what());
    } catch (const error::backtrace_serial_error& e) {
      throw error::backtrace_serial_error(archive["type"].as<std::string>(), e.what());
    } catch (const std::out_of_range&) {
      throw error::undefined_type_reference(archive["type"].as<std::string>());
    }
  }
};
