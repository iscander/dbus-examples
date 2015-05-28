/*
 *   Declarations for Client
 */

#ifndef CALC_CLIENT_H
#define CALC_CLIENT_H

#include <dbus/dbus-glib-bindings.h>

/* Standard GObject class structures, etc */

#define TYPE_CALC_CLIENT                  (calc_client_get_type())
#define CALC_CLIENT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_CALC_CLIENT, CalcClient))
#define IS_CALC_CLIENT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_CALC_CLIENT))
#define CALC_CLIENT_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_CALC_CLIENT, CalcClientClass))
#define IS_CALC_LIENT_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_CALC_CLIENT))
#define CALC_CLIENT_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_CALC_CLIENT, CalcClientClass))

typedef struct
{
  GObjectClass parent_class;
  DBusGConnection *connection;
  DBusGProxy *proxy;
} CalcClientClass;

typedef struct
{
  GObject parnet_instance;
} CalcClient;

GType calc_client_get_type (void);

#include "calc-client-stub.h"
#include "common.h"
#endif /* #ifndef CLIENT_H */
