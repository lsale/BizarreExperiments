#ifndef NAVDATA5_H
#define NAVDATA5_H
#include "navdata.h"

typedef struct _navdata_vision_detect_5_t {
  quint16   tag;
  quint16   size;

  quint32   nb_detected;
  quint32   type[4];
  quint32   xc[4];
  quint32   yc[4];
  quint32   width[4];
  quint32   height[4];
  quint32   dist[4];
  qreal     orientation_angle[4];
}_ATTRIBUTE_PACKED_ navdata_vision_detect_5_t;

typedef struct _navdata_video_stream_t {
  quint16  tag;
  quint16  size;

  quint8 	quant;			// quantizer reference used to encode frame [1:31]
  quint32	frame_size;		// frame size (bytes)
  quint32	frame_number;	// frame index
  quint32	atcmd_ref_seq;  // atmcd ref sequence number
  quint32	atcmd_mean_ref_gap;	// mean time between two consecutive atcmd_ref (ms)
  qreal         atcmd_var_ref_gap;
  quint32	atcmd_ref_quality; // estimator of atcmd link quality
}_ATTRIBUTE_PACKED_ navdata_video_stream_t;

typedef struct _navdata_unpacked_5_t {
  quint32  ardrone_state;
  qint32    vision_defined;
  quint32  last_navdata_refresh;  /*! mask showing which block was refreshed when receiving navdata */

  navdata_demo_t             navdata_demo;
  navdata_time_t             navdata_time;
  navdata_raw_measures_t     navdata_raw_measures;
  navdata_phys_measures_t    navdata_phys_measures;
  navdata_gyros_offsets_t    navdata_gyros_offsets;
  navdata_euler_angles_t     navdata_euler_angles;
  navdata_references_t       navdata_references;
  navdata_trims_t            navdata_trims;
  navdata_rc_references_t    navdata_rc_references;
  navdata_pwm_t              navdata_pwm;
  navdata_altitude_t         navdata_altitude;
  navdata_vision_raw_t       navdata_vision_raw;
  navdata_vision_of_t        navdata_vision_of;
  navdata_vision_t           navdata_vision;
  navdata_vision_perf_t      navdata_vision_perf;
  navdata_trackers_send_t    navdata_trackers_send;
  navdata_vision_detect_5_t  navdata_vision_detect;
  navdata_watchdog_t         navdata_watchdog;
  navdata_adc_data_frame_t   navdata_adc_data_frame;
  navdata_video_stream_t     navdata_video_stream;
} navdata_unpacked_5_t;

static inline bool ardrone_navdata_unpack_all_5(navdata_unpacked_5_t* navdata_unpacked, navdata_t* navdata, quint32* cks)
{
    bool res = true;
    navdata_cks_t navdata_cks = { 0 };
    navdata_option_t* navdata_option_ptr;

    navdata_option_ptr = (navdata_option_t*) &navdata->options[0];

    memset(navdata_unpacked, 0, sizeof(*navdata_unpacked));

    navdata_unpacked->ardrone_state = navdata->ardrone_state;
    navdata_unpacked->vision_defined  = navdata->vision_defined;

    while (navdata_option_ptr != NULL) {
        // Check if we have a valid option
        if (navdata_option_ptr->size == 0) {
            qDebug() << "[NavDataHandler] Error: Option size is zero";
            navdata_option_ptr = NULL;
            res = false;
        } else {
            switch( navdata_option_ptr->tag ) {
            case NAVDATA_DEMO_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_demo);
                break;
            case NAVDATA_TIME_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_time);
                break;
            case NAVDATA_RAW_MEASURES_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_raw_measures);
                break;
            case NAVDATA_PHYS_MEASURES_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_phys_measures);
                break;
            case NAVDATA_GYROS_OFFSETS_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_gyros_offsets);
                break;
            case NAVDATA_EULER_ANGLES_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_euler_angles);
                break;
            case NAVDATA_REFERENCES_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_references);
                break;
            case NAVDATA_TRIMS_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_trims);
                break;
            case NAVDATA_RC_REFERENCES_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_rc_references);
                break;
            case NAVDATA_PWM_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_pwm);
                break;
            case NAVDATA_ALTITUDE_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_altitude);
                break;
            case NAVDATA_VISION_RAW_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_vision_raw);
                break;
            case NAVDATA_VISION_OF_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_vision_of);
                break;
            case NAVDATA_VISION_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_vision);
                break;
            case NAVDATA_VISION_PERF_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_vision_perf);
                break;
            case NAVDATA_TRACKERS_SEND_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_trackers_send);
                break;
            case NAVDATA_VISION_DETECT_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_vision_detect);
                break;
            case NAVDATA_WATCHDOG_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_unpacked->navdata_watchdog);
                break;
            case NAVDATA_ADC_DATA_FRAME_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr,
                        navdata_unpacked->navdata_adc_data_frame);
                break;
            case NAVDATA_VIDEO_STREAM_TAG:
                navdata_option_ptr = ardrone_navdata_unpack( navdata_option_ptr,
                        navdata_unpacked->navdata_video_stream );
                break;
            case NAVDATA_CKS_TAG:
                navdata_option_ptr = ardrone_navdata_unpack(
                        navdata_option_ptr, navdata_cks);
                *cks = navdata_cks.cks;
                navdata_option_ptr = NULL; // End of structure
                break;
            default:
                qDebug() << "[NavDataHandler] Tag "
                         << (int) (navdata_option_ptr->tag)
                         << " is not a valid navdata option tag";
                navdata_option_ptr = NULL;
                res = false;
                break;
            }
        }
    }
    return res;
}
#endif // NAVDATA5_H
