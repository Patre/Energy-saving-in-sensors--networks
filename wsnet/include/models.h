/**
 *  \file   models.h
 *  \brief  Models declarations
 *  \author Guillaume Chelius & Elyes Ben Hamida & Quentin Lampin
 *  \date   2007
 **/
#ifndef __models__
#define __models__

#include <include/types.h>


#define MODELTYPE_PROPAGATION   0
#define MODELTYPE_SHADOWING     1
#define MODELTYPE_FADING        2
#define MODELTYPE_INTERFERENCES 3
#define MODELTYPE_NOISE         4
#define MODELTYPE_MODULATION    5
#define MODELTYPE_ENVIRONMENT   6
#define MODELTYPE_MOBILITY      7
#define MODELTYPE_ANTENNA       8
#define MODELTYPE_RADIO         9
#define MODELTYPE_MAC           10
#define MODELTYPE_ROUTING       11
#define MODELTYPE_APPLICATION   12
#define MODELTYPE_ENERGY        13
#define MODELTYPE_MONITOR       14


/** \typedef model_t
 * \brief Information about a model.
 **/
/** \struct _model
 * \brief Information about a model. Should use type model_t.
 **/
typedef struct _model { /* Model description          */
    char *oneline;      /* one line description       */
    char *author;       /* author                     */
    char *version;      /* version                    */
    int   type;         /* model type                 */
    struct {
        char **exported; /* list of available measures */
        int   count;    /* number of measures          */
    } measure;
} model_t;


/**
 * \typedef propagation_methods_t
 * \brief Methods that should be implemented by a propagation model.
 **/
/**
 * \struct _propagation_methods
 * \brief Methods that should be implemented by a propagation model. Should use type propagation_methods_t.
 **/
typedef struct _propagation_methods {
    double (* propagation) (call_t *c, packet_t *packet, nodeid_t src, nodeid_t dst, double rxdBm);
} propagation_methods_t;


/**
 * \typedef fading_methods_t
 * \brief Methods that should be implemented by a fading model.
 **/
/**
 * \struct _fading_methods
 * \brief Methods that should be implemented by a fading model. Should use type fading_methods_t.
 **/
typedef struct _fading_methods {
    double (* fading) (call_t *c, packet_t *packet, nodeid_t src, nodeid_t dst, double rxdBm);
} fading_methods_t;


/**
 * \typedef shadowing_methods_t
 * \brief Methods that should be implemented by a shadowing model.
 **/
/**
 * \struct _shadowing_methods
 * \brief Methods that should be implemented by a shadowing model. Should use type shadowing_methods_t.
 **/
typedef struct _shadowing_methods {
    double (* shadowing) (call_t *c, packet_t *packet, nodeid_t src, nodeid_t dst, double rxdBm);
} shadowing_methods_t;


/**
 * \typedef interferences_methods_t
 * \brief Methods that should be implemented by a interferences model.
 **/
/**
 * \struct _interferences_methods
 * \brief Methods that should be implemented by a interferences model. Should use type interferences_methods_t.
 **/
typedef struct _interferences_methods {
    double (* interfere) (call_t *c, int channel0, int channel1);
} interferences_methods_t;


/**
 * \typedef noise_methods_t
 * \brief Methods that should be implemented by a noise model.
 **/
/**
 * \struct _noise_methods
 * \brief Methods that should be implemented by a noise model. Should use type noise_methods_t.
 **/
typedef struct _noise_methods {
    double (* noise) (call_t *c, nodeid_t node, int channel);
} noise_methods_t;


/**
 * \typedef modulation_methods_t
 * \brief Methods that should be implemented by a modulation model.
 **/
/**
 * \struct _modulation_methods
 * \brief Methods that should be implemented by a modulation model. Should use type modulation_methods_t.
 **/
typedef struct _modulation_methods {
    double (* modulate) (call_t *c, double snr);
    int (* bit_per_symbol) (call_t *c);
} modulation_methods_t;


/**
 * \typedef environment_methods_t
 * \brief Methods that should be implemented by an environment model.
 **/
/**
 * \struct _environment_methods
 * \brief Methods that should be implemented by an environment model. Should use type environment_methods_t.
 **/
typedef struct _environment_methods {
    void (*read_measure) (call_t *c, measureid_t measure, double *value);
} environment_methods_t;


/**
 * \typedef monitor_methods_t
 * \brief Methods that should be implemented by a monitor model.
 **/
/**
 * \struct _monitor_methods
 * \brief Methods that should be implemented by a monitor model. Should use type monitor_methods_t.
 **/
typedef struct _monitor_methods {
    void (*monitor_death) (call_t *c);
    void (*monitor_event) (call_t *c);
    void (*monitor_register_callback) (call_t *c, callback_t callback, void *arg);
} monitor_methods_t;


/**
 * \typedef mobility_methods_t
 * \brief Methods that should be implemented by a mobility model.
 **/
/**
 * \struct _mobility_methods
 * \brief Methods that should be implemented by a mobility model. Should use type mobility_methods_t.
 **/
typedef struct _mobility_methods {
    void (*update_position) (call_t *c);
} mobility_methods_t;


/**
 * \typedef energy_methods_t
 * \brief Methods that should be implemented by an energy model.
 **/
/**
 * \struct _energy_methods
 * \brief Methods that should be implemented by an energy model. Should use type energy_methods_t.
 **/
typedef struct _energy_methods {
    void (*consume_tx) (call_t *c, uint64_t duration, double txdBm);
    void (*consume_rx) (call_t *c, uint64_t duration);
    void (*consume_idle) (call_t *c, uint64_t duration);
    void (*consume) (call_t *c, double energy);
    double (*energy_consumed) (call_t *c);
    double (*energy_remaining) (call_t *c);
    double (*energy_status) (call_t *c);
} energy_methods_t;


/**
 * \typedef antenna_methods_t
 * \brief Methods that should be implemented by an antenna model.
 **/
/**
 * \struct _antenna_methods
 * \brief Methods that should be implemented by an antenna model. Should use type antenna_methods_t.
 **/
typedef struct _antenna_methods {
    void (*rx) (call_t *c, packet_t *packet);
    void (*cs) (call_t *c, packet_t *packet);
    double (*get_loss) (call_t *c);
    angle_t * (*get_angle) (call_t *c);
    void (*set_angle) (call_t *c, angle_t *angle);
    double (*gain_tx) (call_t *c, position_t *pos);
    double (*gain_rx) (call_t *c, position_t *pos);
} antenna_methods_t;


/**
 * \typedef radio_methods_t
 * \brief Methods that should be implemented by a radio model.
 **/
/**
 * \struct _radio_methods
 * \brief Methods that should be implemented by a radio model. Should use type radio_methods_t.
 **/
typedef struct _radio_methods {
    void (*rx) (call_t *c, packet_t *packet);
    void (*tx) (call_t *c, packet_t *packet);
    /* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
    int (*set_header) (call_t *c, packet_t *packet, destination_t *dst);
    int (*get_header_size) (call_t * c);
    int (*get_header_real_size)(call_t *c);
    /* end of edition */
    void (*tx_end) (call_t *c, packet_t *packet);
    void (*cs) (call_t *c, packet_t *packet);
    double (*get_noise) (call_t *c);
    double (*get_cs) (call_t *c);
    double (*get_power) (call_t *c);
    void (*set_power) (call_t *c, double power);
    int (*get_channel) (call_t *c);
    void (*set_channel) (call_t *c, int channel);
    entityid_t (*get_modulation) (call_t *c);
    void (*set_modulation) (call_t *c, entityid_t modulation);
    uint64_t (*get_Tb) (call_t *c);
    uint64_t (*get_Ts) (call_t *c);
    void (*set_Ts) (call_t *c, uint64_t Ts);
    double (*get_sensibility) (call_t *c);
    void (*set_sensibility) (call_t *c, double sensibility);
    void (*sleep) (call_t *c);
    void (*wakeup) (call_t *c);
    int (*get_modulation_bit_per_symbol) (call_t *c);
} radio_methods_t;


/**
 * \typedef mac_methods_t
 * \brief Methods that should be implemented by a mac model.
 **/
/**
 * \struct _mac_methods
 * \brief Methods that should be implemented by a mac model. Should use type mac_methods_t.
 **/
typedef struct _mac_methods {
    void (*rx) (call_t *c, packet_t *packet);
    void (*tx) (call_t *c, packet_t *packet);
    int (*set_header) (call_t *c, packet_t *packet, destination_t *dst);
    int (*get_header_size) (call_t *c);
    /* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
    int (*get_header_real_size)(call_t *c);
    /* end of edition */
} mac_methods_t;


/**
 * \typedef routing_methods_t
 * \brief Methods that should be implemented by a routing model.
 **/
/**
 * \struct _routing_methods
 * \brief Methods that should be implemented by a routing model. Should use type routing_methods_t.
 **/
typedef struct _routing_methods {
    void (*rx) (call_t *c, packet_t *packet);
    void (*tx) (call_t *c, packet_t *packet);
    int (*set_header) (call_t *c, packet_t *packet, destination_t *dst);
    int (*get_header_size) (call_t *c);
    /* edit by Quentin Lampin <quentin.lampin@orange-ftgroup.com> */
    int (*get_header_real_size)(call_t *c);
    /* end of edition */
} routing_methods_t;


/**
 * \typedef application_methods_t
 * \brief Methods that should be implemented by an application model.
 **/
/**
 * \struct _application_methods
 * \brief Methods that should be implemented by an application model. Should use type application_methods_t.
 **/
typedef struct _application_methods {
    void (*rx) (call_t *c, packet_t *packet);
} application_methods_t;


#endif //__models__
