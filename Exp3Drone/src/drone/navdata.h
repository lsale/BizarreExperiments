#ifndef NAVDATA_H
#define NAVDATA_H

typedef enum {
    FW_UNKNOWN,
    FW_147,
    FW_151
} ardrone_firmware_version;


/* Navdata constant */
#define NAVDATA_SEQUENCE_DEFAULT  1
#define NAVDATA_HEADER            0x55667788
#define FTP_PORT                  5551
#define NAVDATA_PORT              5554
#define VIDEO_PORT                5555
#define AT_PORT                   5556
#define RAW_CAPTURE_PORT          5557

#define DRONE_IP_ADDRESS           "192.168.1.1"

#define NAVDATA_MAX_CUSTOM_TIME_SAVE 20

// NUMBER OF TRACKERS FOR EACH TRACKING
#define NB_CORNER_TRACKERS_WIDTH    5      /* number of trackers in width of current picture */
#define NB_CORNER_TRACKERS_HEIGHT   4      /* number of trackers in height of current picture */

#define DEFAULT_NB_TRACKERS_WIDTH    (NB_CORNER_TRACKERS_WIDTH+1)
#define DEFAULT_NB_TRACKERS_HEIGHT   (NB_CORNER_TRACKERS_HEIGHT+1)

#ifndef _ATTRIBUTE_PACKED_
#define _ATTRIBUTE_PACKED_ __attribute__ ((packed))
#endif

// Bitfield definition for user input
typedef enum {
  ARDRONE_UI_BIT_AG             = 0,
  ARDRONE_UI_BIT_AB             = 1,
  ARDRONE_UI_BIT_AD             = 2,
  ARDRONE_UI_BIT_AH             = 3,
  ARDRONE_UI_BIT_L1             = 4,
  ARDRONE_UI_BIT_R1             = 5,
  ARDRONE_UI_BIT_L2             = 6,
  ARDRONE_UI_BIT_R2             = 7,
  ARDRONE_UI_BIT_SELECT         = 8,
  ARDRONE_UI_BIT_START          = 9,
  ARDRONE_UI_BIT_TRIM_THETA     = 18,
  ARDRONE_UI_BIT_TRIM_PHI       = 20,
  ARDRONE_UI_BIT_TRIM_YAW       = 22,
  ARDRONE_UI_BIT_X              = 24,
  ARDRONE_UI_BIT_Y              = 28
} ardrone_ui_bitfield_t;

// Define constants for gyrometers handling
typedef enum {
  GYRO_X    = 0,
  GYRO_Y    = 1,
  GYRO_Z    = 2,
  NB_GYROS  = 3
} def_gyro_t;


// Define constants for accelerometers handling
typedef enum {
  ACC_X   = 0,
  ACC_Y   = 1,
  ACC_Z   = 2,
  NB_ACCS = 3
} def_acc_t;

typedef enum _navdata_tag_t {
  NAVDATA_DEMO_TAG = 0,
  NAVDATA_TIME_TAG,
  NAVDATA_RAW_MEASURES_TAG,
  NAVDATA_PHYS_MEASURES_TAG,
  NAVDATA_GYROS_OFFSETS_TAG,
  NAVDATA_EULER_ANGLES_TAG,
  NAVDATA_REFERENCES_TAG,
  NAVDATA_TRIMS_TAG,
  NAVDATA_RC_REFERENCES_TAG,
  NAVDATA_PWM_TAG,
  NAVDATA_ALTITUDE_TAG,
  NAVDATA_VISION_RAW_TAG,
  NAVDATA_VISION_OF_TAG,
  NAVDATA_VISION_TAG,
  NAVDATA_VISION_PERF_TAG,
  NAVDATA_TRACKERS_SEND_TAG,
  NAVDATA_VISION_DETECT_TAG,
  NAVDATA_WATCHDOG_TAG,
  NAVDATA_ADC_DATA_FRAME_TAG,
  NAVDATA_VIDEO_STREAM_TAG,
  /*Insert new tags above this line */
  NAVDATA_NUM_TAGS,
  NAVDATA_CKS_TAG = 0xFFFF
} navdata_tag_t;

typedef struct _matrix33_t
{
    qreal m11;
    qreal m12;
    qreal m13;
    qreal m21;
    qreal m22;
    qreal m23;
    qreal m31;
    qreal m32;
    qreal m33;
} matrix33_t;

typedef union _float_or_int_t {
  qreal f;
  qint32   i;
} float_or_int_t;


typedef struct _vector31_t {
    union {
        qreal v[3];
        struct
        {
            qreal x;
            qreal y;
            qreal z;
        };
    };
} vector31_t;

typedef struct _navdata_option_t {
    quint16  tag;
    quint16  size;

    quint8   data[];
} navdata_option_t;


typedef struct _navdata_t {
  quint32    header;
  quint32    ardrone_state;
  quint32    sequence;
  bool  	 vision_defined;

  navdata_option_t  options[1];
}_ATTRIBUTE_PACKED_ navdata_t;


typedef struct _navdata_demo_t {
  quint16    tag;					  /*!< Navdata block ('option') identifier */
  quint16    size;					  /*!< set this to the size of this structure */

  quint32    ctrl_state;             /*!< Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum. */
  quint32    vbat_flying_percentage; /*!< battery voltage filtered (mV) */

  qreal   theta;                  /*!< UAV's pitch in milli-degrees */
  qreal   phi;                    /*!< UAV's roll  in milli-degrees */
  qreal   psi;                    /*!< UAV's yaw   in milli-degrees */

  qint32     altitude;               /*!< UAV's altitude in centimeters */

  qreal   vx;                     /*!< UAV's estimated linear velocity */
  qreal   vy;                     /*!< UAV's estimated linear velocity */
  qreal   vz;                     /*!< UAV's estimated linear velocity */

  quint32    num_frames;			  /*!< streamed frame index */ // Not used -> To integrate in video stage.

  // Camera parameters compute by detection
  matrix33_t  detection_camera_rot;   /*!<  Deprecated ! Don't use ! */
  vector31_t  detection_camera_trans; /*!<  Deprecated ! Don't use ! */
  quint32	  detection_tag_index;    /*!<  Deprecated ! Don't use ! */

  quint32	  detection_camera_type; /*!<  Type of tag searched in detection */

  // Camera parameters compute by drone
  matrix33_t  drone_camera_rot;		/*!<  Deprecated ! Don't use ! */
  vector31_t  drone_camera_trans;	/*!<  Deprecated ! Don't use ! */
}_ATTRIBUTE_PACKED_ navdata_demo_t;

/// Last navdata option that *must* be included at the end of all navdata packet
/// + 6 bytes
typedef struct _navdata_cks_t {
  quint16  tag;
  quint16  size;

  // Checksum for all navdatas (including options)
  quint32  cks;
}_ATTRIBUTE_PACKED_ navdata_cks_t;


/// + 6 bytes
typedef struct _navdata_time_t {
  quint16  tag;
  quint16  size;

  quint32  time;  /*!< 32 bit value where the 11 most significant bits represents the seconds, and the 21 least significant bits are the microseconds. */
}_ATTRIBUTE_PACKED_ navdata_time_t;

/**
 * \struct _velocities_t
 * \brief Velocities in qreal format
 */
typedef struct _velocities_t {
  qreal x;
  qreal y;
  qreal z;
} velocities_t;

typedef struct _navdata_raw_measures_t {
  quint16  tag;
  quint16  size;

  // +12 bytes
  quint16  raw_accs[NB_ACCS];    // filtered accelerometers
  quint16  raw_gyros[NB_GYROS];  // filtered gyrometers
  quint16  raw_gyros_110[2];     // gyrometers  x/y 110 deg/s
  quint32  vbat_raw;             // battery voltage raw (mV)
  quint16  us_debut_echo;
  quint16  us_fin_echo;
  quint16  us_association_echo;
  quint16  us_distance_echo;
  quint16  us_courbe_temps;
  quint16  us_courbe_valeur;
  quint16  us_courbe_ref;
}_ATTRIBUTE_PACKED_ navdata_raw_measures_t;

typedef struct _navdata_phys_measures_t {
  quint16   tag;
  quint16   size;

  qreal   accs_temp;
  quint16    gyro_temp;
  qreal   phys_accs[NB_ACCS];
  qreal   phys_gyros[NB_GYROS];
  quint32    alim3V3;              // 3.3volt alim [LSB]
  quint32    vrefEpson;            // ref volt Epson gyro [LSB]
  quint32    vrefIDG;              // ref volt IDG gyro [LSB]
}_ATTRIBUTE_PACKED_ navdata_phys_measures_t;


typedef struct _navdata_gyros_offsets_t {
  quint16   tag;
  quint16   size;

  qreal offset_g[NB_GYROS];
}_ATTRIBUTE_PACKED_ navdata_gyros_offsets_t;


typedef struct _navdata_euler_angles_t {
  quint16   tag;
  quint16   size;

  qreal   theta_a;
  qreal   phi_a;
}_ATTRIBUTE_PACKED_ navdata_euler_angles_t;


typedef struct _navdata_references_t {
  quint16   tag;
  quint16   size;

  qint32   ref_theta;
  qint32   ref_phi;
  qint32   ref_theta_I;
  qint32   ref_phi_I;
  qint32   ref_pitch;
  qint32   ref_roll;
  qint32   ref_yaw;
  qint32   ref_psi;
}_ATTRIBUTE_PACKED_ navdata_references_t;


typedef struct _navdata_trims_t {
  quint16   tag;
  quint16   size;

  qreal angular_rates_trim_r;
  qreal euler_angles_trim_theta;
  qreal euler_angles_trim_phi;
}_ATTRIBUTE_PACKED_ navdata_trims_t;

typedef struct _navdata_rc_references_t {
  quint16   tag;
  quint16   size;

  qint32    rc_ref_pitch;
  qint32    rc_ref_roll;
  qint32    rc_ref_yaw;
  qint32    rc_ref_gaz;
  qint32    rc_ref_ag;
}_ATTRIBUTE_PACKED_ navdata_rc_references_t;


typedef struct _navdata_pwm_t {
  quint16   tag;
  quint16   size;

  quint8     motor1;
  quint8     motor2;
  quint8     motor3;
  quint8     motor4;
  quint8	  sat_motor1;
  quint8	  sat_motor2;
  quint8	  sat_motor3;
  quint8	  sat_motor4;
  qint32     gaz_feed_forward;
  qint32     gaz_altitude;
  qreal   altitude_integral;
  qreal   vz_ref;
  qint32     u_pitch;
  qint32     u_roll;
  qint32     u_yaw;
  qreal   yaw_u_I;
  qint32     u_pitch_planif;
  qint32     u_roll_planif;
  qint32     u_yaw_planif;
  qreal   u_gaz_planif;
  quint16    current_motor1;
  quint16    current_motor2;
  quint16    current_motor3;
  quint16    current_motor4;
}_ATTRIBUTE_PACKED_ navdata_pwm_t;


typedef struct _navdata_altitude_t {
  quint16   tag;
  quint16   size;

  qint32   altitude_vision;
  qreal altitude_vz;
  qint32   altitude_ref;
  qint32   altitude_raw;
}_ATTRIBUTE_PACKED_ navdata_altitude_t;


typedef struct _navdata_vision_raw_t {
  quint16   tag;
  quint16   size;

  qreal vision_tx_raw;
  qreal vision_ty_raw;
  qreal vision_tz_raw;
}_ATTRIBUTE_PACKED_ navdata_vision_raw_t;


typedef struct _navdata_vision_t {
  quint16   tag;
  quint16   size;

  quint32   vision_state;
  qint32    vision_misc;
  qreal  vision_phi_trim;
  qreal  vision_phi_ref_prop;
  qreal  vision_theta_trim;
  qreal  vision_theta_ref_prop;

  qint32    new_raw_picture;
  qreal  theta_capture;
  qreal  phi_capture;
  qreal  psi_capture;
  qint32    altitude_capture;
  quint32   time_capture;
  velocities_t body_v;

  qreal  delta_phi;
  qreal  delta_theta;
  qreal  delta_psi;

  quint32	gold_defined;
  quint32	gold_reset;
  qreal gold_x;
  qreal gold_y;
}_ATTRIBUTE_PACKED_ navdata_vision_t;


typedef struct _navdata_vision_perf_t {
  quint16   tag;
  quint16   size;

  // +44 bytes
  qreal  time_szo;
  qreal  time_corners;
  qreal  time_compute;
  qreal  time_tracking;
  qreal  time_trans;
  qreal  time_update;
  qreal  time_custom[NAVDATA_MAX_CUSTOM_TIME_SAVE];
}_ATTRIBUTE_PACKED_ navdata_vision_perf_t;

typedef struct _screen_point_t {
  qint32 x;
  qint32 y;
} screen_point_t;

typedef struct _navdata_trackers_send_t {
  quint16   tag;
  quint16   size;

  qint32 locked[DEFAULT_NB_TRACKERS_WIDTH * DEFAULT_NB_TRACKERS_HEIGHT];
  screen_point_t point[DEFAULT_NB_TRACKERS_WIDTH * DEFAULT_NB_TRACKERS_HEIGHT];
}_ATTRIBUTE_PACKED_ navdata_trackers_send_t;


typedef struct _navdata_vision_of_t {
  quint16   tag;
  quint16   size;

  qreal   of_dx[5];
  qreal   of_dy[5];
}_ATTRIBUTE_PACKED_ navdata_vision_of_t;


typedef struct _navdata_watchdog_t {
  quint16   tag;
  quint16   size;

  // +4 bytes
  qint32    watchdog;
}_ATTRIBUTE_PACKED_ navdata_watchdog_t;

typedef struct _navdata_adc_data_frame_t {
  quint16  tag;
  quint16  size;

  quint32  version;
  quint8   data_frame[32];
}_ATTRIBUTE_PACKED_ navdata_adc_data_frame_t;

static inline int get_mask_from_state( quint32 state, quint32 mask )
{
    return state & mask ? TRUE : FALSE;
}

static inline quint8* navdata_unpack_option( quint8* navdata_ptr, quint8* data, quint32 size )
{
    memcpy(data, navdata_ptr, size);

    return (navdata_ptr + size);
}

static inline quint32 navdata_compute_cks( quint8* nv, qint32 size )
{
    qint32 i;
    quint32 cks;
    quint32 temp;

    cks = 0;

    for( i = 0; i < size; i++ )
        {
            temp = nv[i];
            cks += temp;
        }

    return cks;
}

/*************************From app.h**************************/
enum {
  NO_CONTROL_MODE = 0,          // Doing nothing
  MYKONOS_UPDATE_CONTROL_MODE,  // Mykonos software update reception (update is done next run)
                                // After event completion, card should power off
  PIC_UPDATE_CONTROL_MODE,      // Mykonos pic software update reception (update is done next run)
                                // After event completion, card should power off
  LOGS_GET_CONTROL_MODE,        // Send previous run's logs
  CFG_GET_CONTROL_MODE,         // Send activ configuration
  ACK_CONTROL_MODE              // Reset command mask in navdata
};


/// \enum def_ardrone_state_mask_t is a bit field representing ARDrone' state


// Define masks for ARDrone state
// 31                                                             0
//  x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x -> state
//  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | FLY MASK : (0) ardrone is landed, (1) ardrone is flying
//  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | VIDEO MASK : (0) video disable, (1) video enable
//  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | VISION MASK : (0) vision disable, (1) vision enable
//  | | | | | | | | | | | | | | | | | | | | | | | | | | | | CONTROL ALGO : (0) euler angles control, (1) angular speed control
//  | | | | | | | | | | | | | | | | | | | | | | | | | | | ALTITUDE CONTROL ALGO : (0) altitude control inactive (1) altitude control active
//  | | | | | | | | | | | | | | | | | | | | | | | | | | USER feedback : Start button state
//  | | | | | | | | | | | | | | | | | | | | | | | | | Control command ACK : (0) None, (1) one received
//  | | | | | | | | | | | | | | | | | | | | | | | | Trim command ACK : (0) None, (1) one received
//  | | | | | | | | | | | | | | | | | | | | | | | Trim running : (0) none, (1) running
//  | | | | | | | | | | | | | | | | | | | | | | Trim result : (0) failed, (1) succeeded
//  | | | | | | | | | | | | | | | | | | | | | Navdata demo : (0) All navdata, (1) only navdata demo
//  | | | | | | | | | | | | | | | | | | | | Navdata bootstrap : (0) options sent in all or demo mode, (1) no navdata options sent
//  | | | | | | | | | | | | | | | | | | |
//  | | | | | | | | | | | | | | | | | |
//  | | | | | | | | | | | | | | | | | Bit means that there's an hardware problem with gyrometers
//  | | | | | | | | | | | | | | | | VBat low : (1) too low, (0) Ok
//  | | | | | | | | | | | | | | | VBat high (US mad) : (1) too high, (0) Ok
//  | | | | | | | | | | | | | | Timer elapsed : (1) elapsed, (0) not elapsed
//  | | | | | | | | | | | | | Power : (0) Ok, (1) not enough to fly
//  | | | | | | | | | | | | Angles : (0) Ok, (1) out of range
//  | | | | | | | | | | | Wind : (0) Ok, (1) too much to fly
//  | | | | | | | | | | Ultrasonic sensor : (0) Ok, (1) deaf
//  | | | | | | | | | Cutout system detection : (0) Not detected, (1) detected
//  | | | | | | | | PIC Version number OK : (0) a bad version number, (1) version number is OK
//  | | | | | | | ATCodec thread ON : (0) thread OFF (1) thread ON
//  | | | | | | Navdata thread ON : (0) thread OFF (1) thread ON
//  | | | | | Video thread ON : (0) thread OFF (1) thread ON
//  | | | | Acquisition thread ON : (0) thread OFF (1) thread ON
//  | | | CTRL watchdog : (1) delay in control execution (> 5ms), (0) control is well scheduled // Check frequency of control loop
//  | | ADC Watchdog : (1) delay in uart2 dsr (> 5ms), (0) uart2 is good // Check frequency of uart2 dsr (com with adc)
//  | Communication Watchdog : (1) com problem, (0) Com is ok // Check if we have an active connection with a client
//  Emergency landing : (0) no emergency, (1) emergency

typedef enum {
  ARDRONE_FLY_MASK            = 1 << 0,  /*!< FLY MASK : (0) ardrone is landed, (1) ardrone is flying */
  ARDRONE_VIDEO_MASK          = 1 << 1,  /*!< VIDEO MASK : (0) video disable, (1) video enable */
  ARDRONE_VISION_MASK         = 1 << 2,  /*!< VISION MASK : (0) vision disable, (1) vision enable */
  ARDRONE_CONTROL_MASK        = 1 << 3,  /*!< CONTROL ALGO : (0) euler angles control, (1) angular speed control */
  ARDRONE_ALTITUDE_MASK       = 1 << 4,  /*!< ALTITUDE CONTROL ALGO : (0) altitude control inactive (1) altitude control active */
  ARDRONE_USER_FEEDBACK_START = 1 << 5,  /*!< USER feedback : Start button state */
  ARDRONE_COMMAND_MASK        = 1 << 6,  /*!< Control command ACK : (0) None, (1) one received */
  ARDRONE_FW_FILE_MASK        = 1 << 7,  /* Firmware file is good (1) */
  ARDRONE_FW_VER_MASK         = 1 << 8,  /* Firmware update is newer (1) */
//  ARDRONE_FW_UPD_MASK         = 1 << 9,  /* Firmware update is ongoing (1) */
  ARDRONE_NAVDATA_DEMO_MASK   = 1 << 10, /*!< Navdata demo : (0) All navdata, (1) only navdata demo */
  ARDRONE_NAVDATA_BOOTSTRAP   = 1 << 11, /*!< Navdata bootstrap : (0) options sent in all or demo mode, (1) no navdata options sent */
  ARDRONE_MOTORS_MASK  	      = 1 << 12, /*!< Motors status : (0) Ok, (1) Motors problem */
  ARDRONE_COM_LOST_MASK       = 1 << 13, /*!< Communication Lost : (1) com problem, (0) Com is ok */
  ARDRONE_VBAT_LOW            = 1 << 15, /*!< VBat low : (1) too low, (0) Ok */
  ARDRONE_USER_EL             = 1 << 16, /*!< User Emergency Landing : (1) User EL is ON, (0) User EL is OFF*/
  ARDRONE_TIMER_ELAPSED       = 1 << 17, /*!< Timer elapsed : (1) elapsed, (0) not elapsed */
  ARDRONE_ANGLES_OUT_OF_RANGE = 1 << 19, /*!< Angles : (0) Ok, (1) out of range */
  ARDRONE_ULTRASOUND_MASK     = 1 << 21, /*!< Ultrasonic sensor : (0) Ok, (1) deaf */
  ARDRONE_CUTOUT_MASK         = 1 << 22, /*!< Cutout system detection : (0) Not detected, (1) detected */
  ARDRONE_PIC_VERSION_MASK    = 1 << 23, /*!< PIC Version number OK : (0) a bad version number, (1) version number is OK */
  ARDRONE_ATCODEC_THREAD_ON   = 1 << 24, /*!< ATCodec thread ON : (0) thread OFF (1) thread ON */
  ARDRONE_NAVDATA_THREAD_ON   = 1 << 25, /*!< Navdata thread ON : (0) thread OFF (1) thread ON */
  ARDRONE_VIDEO_THREAD_ON     = 1 << 26, /*!< Video thread ON : (0) thread OFF (1) thread ON */
  ARDRONE_ACQ_THREAD_ON       = 1 << 27, /*!< Acquisition thread ON : (0) thread OFF (1) thread ON */
  ARDRONE_CTRL_WATCHDOG_MASK  = 1 << 28, /*!< CTRL watchdog : (1) delay in control execution (> 5ms), (0) control is well scheduled */
  ARDRONE_ADC_WATCHDOG_MASK   = 1 << 29, /*!< ADC Watchdog : (1) delay in uart2 dsr (> 5ms), (0) uart2 is good */
  ARDRONE_COM_WATCHDOG_MASK   = 1 << 30, /*!< Communication Watchdog : (1) com problem, (0) Com is ok */
  ARDRONE_EMERGENCY_MASK      = 1 << 31  /*!< Emergency landing : (0) no emergency, (1) emergency */
} def_ardrone_state_mask_t;


#define ardrone_navdata_unpack( navdata_ptr, option ) (navdata_option_t*) navdata_unpack_option( \
                                                                         (quint8*) navdata_ptr, \
                                                                         (quint8*) &option,     \
                                                                          navdata_ptr->size )

#endif // NAVDATA_H
