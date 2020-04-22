/***************************************************************************//**
 * @file
 * @brief Silicon Labs iBeacon Demo Application
 * This application is intended to be used with the iOS and Android Silicon Labs
 * app for demonstration purposes
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* BG stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "infrastructure.h"

/* libraries containing default gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#ifdef FEATURE_BOARD_DETECTED
#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif
#endif

#include "rail_chip_specific.h"

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

/* Gecko configuration parameters (see gecko_configuration.h) */
static const gecko_configuration_t config = {
  .config_flags = 0,
#if defined(FEATURE_LFXO)
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
#else
  .sleep.flags = 0,
#endif // LFXO
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.sleep_clock_accuracy = 100, // ppm
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap),
  .gattdb = &bg_gattdb_data,
  .pa.config_enable = 1, // Set this to be a valid PA config
  .pa.pa_mode = RAIL_TX_POWER_MODE_2P4_LP,
#if defined(FEATURE_PA_INPUT_FROM_VBAT)
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#else
  .pa.input = GECKO_RADIO_PA_INPUT_DCDC,
#endif // defined(FEATURE_PA_INPUT_FROM_VBAT)
  .rf.flags = GECKO_RF_CONFIG_ANTENNA,                 /* Enable antenna configuration. */
  .rf.antenna = GECKO_RF_ANTENNA,                      /* Select antenna path! */
};

/**
 * @brief Function for creating a custom advertisement package
 *
 * The function builds the advertisement package according to Apple iBeacon specifications,
 * configures this as the device advertisement data and starts broadcasting.
 */
void bcnSetupAdvBeaconing(void)
{
  /* This function sets up a custom advertisement package according to iBeacon specifications.
   * The advertisement package is 30 bytes long. See the iBeacon specification for further details.
   */
#define EDDYSTONE_DATA_LEN           (24)

static const uint8_t eddystone_data[EDDYSTONE_DATA_LEN] = {
  0x03,          // Length of service list
  0x03,          // Service list
  0xAA, 0xFE,    // Eddystone ID (Can change the AA value for multiple beacons)
  0x13,          // Length of service data
  0x16,          // Service data
  0xAA,  0xFE,   // Eddystone ID
  0x10,          // Frame type Eddystone-URL
  0x00,          // Tx power
  0x03,          // http://www., 0x01=https://www.
  'b','i','t','.','l','y','/','n','g','0','0','0','8'
};

  /* Set 0 dBm Transmit Power */
  gecko_cmd_system_set_tx_power(0);

  /* Set custom advertising data */
  gecko_cmd_le_gap_bt5_set_adv_data(0, 0, EDDYSTONE_DATA_LEN, eddystone_data);

  /* Set advertising parameters. 100ms advertisement interval.
   * The first two parameters are minimum and maximum advertising interval,
   * both in units of (milliseconds * 1.6). */
  gecko_cmd_le_gap_set_advertise_timing(0, 160, 160, 0, 0);

  /* Start advertising in user mode and enable connections */
  gecko_cmd_le_gap_start_advertising(0, le_gap_user_data, le_gap_non_connectable);
}

/**
 * @brief  Main function
 */
int main(void)
{
  // Initialize device
  initMcu();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();
  initVcomEnable();
  // Initialize stack
  gecko_init(&config);

  while (1) {
    struct gecko_cmd_packet* evt;

    // Check for stack event.
    evt = gecko_wait_event();

    // Run application and event handler.
    switch (BGLIB_MSG_ID(evt->header)) {
      // This boot event is generated when the system boots up after reset.
      // Do not call any stack commands before receiving the boot event.
      case gecko_evt_system_boot_id:
        // Initialize iBeacon ADV data
        bcnSetupAdvBeaconing();
        break;
      case gecko_evt_le_connection_opened_id:

    	break;
      default:
        break;
    }
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */