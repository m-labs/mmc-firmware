/*
 *   openMMC -- Open Source modular IPM Controller firmware
 *
 *   Copyright (C) 2015-2016  Henrique Silva <henrique.silva@lnls.br>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

/* Project Includes */
#include "sdr.h"
#include "utils.h"

/* Rename file to sdr_list.c */

#ifdef MODULE_XR77129_VOLTAGE
#include "xr77129.h"
#define SDR_RTM_EXAR_P1V0_ID	"RTM EXAR 1V0"
#define SDR_RTM_EXAR_P3V3_ID	"RTM EXAR 3V3"
#define SDR_RTM_EXAR_P1V8_ID	"RTM EXAR 1V8"
#define SDR_RTM_EXAR_P1V2_ID	"RTM EXAR 1V2"
#endif

#ifdef MODULE_LM75
const SDR_type_01h_t SDR_LM75_RTM_1 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM1"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM1" /* sensor string */
};

const SDR_type_01h_t SDR_LM75_RTM_2 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM2"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM2" /* sensor string */
};

const SDR_type_01h_t SDR_LM75_RTM_MEZZ_1 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM MEZZ 1"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM MEZZ 1" /* sensor string */
};

const SDR_type_01h_t SDR_LM75_RTM_MEZZ_2 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM MEZZ 2"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM MEZZ 2" /* sensor string */
};

const SDR_type_01h_t SDR_LM75_RTM_MEZZ_3 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM MEZZ 3"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM MEZZ 3" /* sensor string */
};

const SDR_type_01h_t SDR_LM75_RTM_MEZZ_4 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM MEZZ 4"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM MEZZ 4" /* sensor string */
};

const SDR_type_01h_t SDR_LM75_RTM_MEZZ_CLK = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM CLK"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM CLK" /* sensor string */
};
#endif

#ifdef MODULE_ADT7420
const SDR_type_01h_t SDR_ADT7420_RTM_MEZZ_1 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM MEZZ 1"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM MEZZ 1" /* sensor string */
};

const SDR_type_01h_t SDR_ADT7420_RTM_MEZZ_2 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM MEZZ 2"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM MEZZ 2" /* sensor string */
};

const SDR_type_01h_t SDR_ADT7420_RTM_MEZZ_3 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM MEZZ 3"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM MEZZ 3" /* sensor string */
};

const SDR_type_01h_t SDR_ADT7420_RTM_MEZZ_4 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC0, /* entity id: RTM */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_TEMPERATURE, /* sensor type */
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x01, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 5, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x00, /* Sensor direction */
    .Rexp_Bexp = 0xF0, /* R-Exp , B-Exp */
    .analog_flags = 0x03, /* Analogue characteristics flags */
    .nominal_reading = (30 << 1), /* Nominal reading */
    .normal_max = (50 << 1), /* Normal maximum */
    .normal_min = (20 << 1), /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = (75 << 1), /* Upper non-recoverable Threshold */
    .upper_critical_thr = (65 << 1), /* Upper critical Threshold */
    .upper_noncritical_thr = (55 << 1), /* Upper non critical Threshold */
    .lower_nonrecover_thr = (5 << 1), /* Lower non-recoverable Threshold */
    .lower_critical_thr = (10 << 1), /* Lower critical Threshold */
    .lower_noncritical_thr = (20 << 1), /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE("TEMP RTM MEZZ 4"), /* 8 bit ASCII, number of bytes */
    .IDstring = "TEMP RTM MEZZ 4" /* sensor string */
};
#endif

#ifdef MODULE_XR77129_VOLTAGE
const SDR_type_01h_t SDR_RTM_EXAR_P1V0 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC1, /* entity id: AMC Module */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_VOLTAGE, /* sensor type: Voltage*/
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x04, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 32, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x02, /* Sensor direction */
    .Rexp_Bexp = 0xD0, /* R-Exp = -3 , B-Exp = 0 */
    .analog_flags = 0x07, /* Analogue characteristics flags */
    .nominal_reading = 31, /* Nominal reading = 1.0 V */
    .normal_max = 33, /* Normal maximum */
    .normal_min = 0, /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = 34, /* Upper non-recoverable Threshold */
    .upper_critical_thr = 33, /* Upper critical Threshold */
    .upper_noncritical_thr = 32, /* Upper non critical Threshold */
    .lower_nonrecover_thr = 28, /* Lower non-recoverable Threshold */
    .lower_critical_thr = 29, /* Lower critical Threshold */
    .lower_noncritical_thr = 30, /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE(SDR_RTM_EXAR_1V0_ID) , /* 8 bit ASCII, number of bytes */
    .IDstring = SDR_RTM_EXAR_1V0_ID /* sensor string */
};

const SDR_type_01h_t SDR_RTM_EXAR_P3V3 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC1, /* entity id: AMC Module */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_VOLTAGE, /* sensor type: Voltage*/
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x04, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 64, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x02, /* Sensor direction */
    .Rexp_Bexp = 0xD0, /* R-Exp = -3 , B-Exp = 0 */
    .analog_flags = 0x07, /* Analogue characteristics flags */
    .nominal_reading = 52, /* Nominal reading = 3.3 V */
    .normal_max = 59, /* Normal maximum */
    .normal_min = 45, /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = 62, /* Upper non-recoverable Threshold */
    .upper_critical_thr = 59, /* Upper critical Threshold */
    .upper_noncritical_thr = 55, /* Upper non critical Threshold */
    .lower_nonrecover_thr = 42, /* Lower non-recoverable Threshold */
    .lower_critical_thr = 45, /* Lower critical Threshold */
    .lower_noncritical_thr = 48, /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE(SDR_RTM_EXAR_3V3_ID) , /* 8 bit ASCII, number of bytes */
    .IDstring = SDR_RTM_EXAR_3V3_ID /* sensor string */
};

const SDR_type_01h_t SDR_RTM_EXAR_P1V8 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC1, /* entity id: AMC Module */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_VOLTAGE, /* sensor type: Voltage*/
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x04, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 64, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x02, /* Sensor direction */
    .Rexp_Bexp = 0xD0, /* R-Exp = -3 , B-Exp = 0 */
    .analog_flags = 0x07, /* Analogue characteristics flags */
    .nominal_reading = 28, /* Nominal reading = 1.0 V */
    .normal_max = 29, /* Normal maximum */
    .normal_min = 27, /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = 30, /* Upper non-recoverable Threshold */
    .upper_critical_thr = 29, /* Upper critical Threshold */
    .upper_noncritical_thr = 29, /* Upper non critical Threshold */
    .lower_nonrecover_thr = 26, /* Lower non-recoverable Threshold */
    .lower_critical_thr = 27, /* Lower critical Threshold */
    .lower_noncritical_thr = 27, /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE(SDR_RTM_EXAR_1V8_ID) , /* 8 bit ASCII, number of bytes */
    .IDstring = SDR_RTM_EXAR_1V8_ID /* sensor string */
};

const SDR_type_01h_t SDR_EXAR_P1V2 = {

    .hdr.recID_LSB = 0x00, /* Filled by sdr_insert_entry() */
    .hdr.recID_MSB = 0x00,
    .hdr.SDRversion = 0x51,
    .hdr.rectype = TYPE_01,
    .hdr.reclength = sizeof(SDR_type_01h_t) - sizeof(SDR_entry_hdr_t),

    .ownerID = 0x00, /* i2c address, -> SDR_Init */
    .ownerLUN = 0x00, /* sensor owner LUN */
    .sensornum = 0x00, /* Filled by sdr_insert_entry() */

    /* record body bytes */
    .entityID = 0xC1, /* entity id: AMC Module */
    .entityinstance = 0x00, /* entity instance -> SDR_Init */
    .sensorinit = 0x7f, /* init: event generation + scanning enabled */
    .sensorcap = 0x68, /* capabilities: auto re-arm,*/
    .sensortype = SENSOR_TYPE_VOLTAGE, /* sensor type: Voltage*/
    .event_reading_type = 0x01, /* sensor reading*/
    .assertion_event_mask = { 0xFF, /* LSB assert event mask: 3 bit value */
                              0x0F }, /* MSB assert event mask */
    .deassertion_event_mask = { 0xFF, /* LSB deassert event mask: 3 bit value */
                                0x0F }, /* MSB deassert event mask */
    .readable_threshold_mask = 0x3F, /* LSB: readabled Threshold mask: all thresholds are readabled:  */
    .settable_threshold_mask = 0x3F, /* MSB: setabled Threshold mask: all thresholds are setabled: */
    .sensor_units_1 = 0x00, /* sensor units 1 :*/
    .sensor_units_2 = 0x04, /* sensor units 2 :*/
    .sensor_units_3 = 0x00, /* sensor units 3 :*/
    .linearization = 0x00, /* Linearization */
    .M = 32, /* M */
    .M_tol = 0x00, /* M - Tolerance */
    .B = 0x00, /* B */
    .B_accuracy = 0x00, /* B - Accuracy */
    .acc_exp_sensor_dir = 0x02, /* Sensor direction */
    .Rexp_Bexp = 0xD0, /* R-Exp = -3 , B-Exp = 0 */
    .analog_flags = 0x07, /* Analogue characteristics flags */
    .nominal_reading = 37, /* Nominal reading = 1.0 V */
    .normal_max = 39, /* Normal maximum */
    .normal_min = 35, /* Normal minimum */
    .sensor_max_reading = 0xFF, /* Sensor Maximum reading */
    .sensor_min_reading = 0x00, /* Sensor Minimum reading */
    .upper_nonrecover_thr = 41, /* Upper non-recoverable Threshold */
    .upper_critical_thr = 40, /* Upper critical Threshold */
    .upper_noncritical_thr = 39, /* Upper non critical Threshold */
    .lower_nonrecover_thr = 33, /* Lower non-recoverable Threshold */
    .lower_critical_thr = 34, /* Lower critical Threshold */
    .lower_noncritical_thr = 35, /* Lower non-critical Threshold */
    .pos_thr_hysteresis = 2, /* positive going Threshold hysteresis value */
    .neg_thr_hysteresis = 2, /* negative going Threshold hysteresis value */
    .reserved1 = 0x00, /* reserved */
    .reserved2 = 0x00, /* reserved */
    .OEM = 0x00, /* OEM reserved */
    .IDtypelen = 0xc0 | STR_SIZE(SDR_RTM_EXAR_1V2_ID) , /* 8 bit ASCII, number of bytes */
    .IDstring = SDR_RTM_EXAR_1V2_ID /* sensor string */
};
#endif

void rtm_sdr_init( void )
{
#ifdef MODULE_HOTSWAP
//    sdr_insert_entry( TYPE_02, (void *) &SDR_HOTSWAP_RTM, &vTaskHotSwap_Handle, 0, 0 );
#endif

#ifdef MODULE_LM75
//    sdr_insert_entry( TYPE_01, (void *) &SDR_LM75_RTM_1, &vTaskLM75_Handle, 0, CHIP_ID_RTM_LM75_0 );
//    sdr_insert_entry( TYPE_01, (void *) &SDR_LM75_RTM_2, &vTaskLM75_Handle, 0, CHIP_ID_RTM_LM75_1 );
#endif
}
