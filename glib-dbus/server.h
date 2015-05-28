/* 
 *   Declarations for Server
 */

#ifndef CALC_SERVER_H
#define CALC_SERVER_H

#include <dbus/dbus-glib-bindings.h>

/* Standard GObject class structures, etc */

GType calc_server_get_type (void);
#define TYPE_CALC_SERVER                  (calc_server_get_type())
#define CALC_SERVER(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_CALC_SERVER, CalcServer))
#define IS_CALC_SERVER(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_CALC_SERVER))
#define CALC_SERVER_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_CALC_SERVER, CalcServerClass))
#define IS_CALC_SERVER_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_CALC_SERVER))
#define CALC_SERVER_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_CALC_SERVER, CalcServerClass))

typedef struct
{
  GObjectClass parent_class;
  DBusGConnection *connection;
} CalcServerClass;

typedef struct
{
  GObject parent_instance;
} CalcServer;


gboolean calc_add (CalcServer * obj, gint first, gint second, gint * sum,
		   GError ** error);
gboolean calc_sub (CalcServer * obj, gint minued, gint sub, gint * diff,
		   GError ** error);

#include "calc-server-stub.h"
#include "common.h"
#endif /* #ifndef SERVER_H */
