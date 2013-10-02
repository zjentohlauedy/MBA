#ifndef __INC_SERVICES_H__
#define __INC_SERVICES_H__

#define TRY( Func ) if ( (rc = (Func)) != SQLITE_OK ) return rc
#define INSERT_IF_UNIQUE( Func ) if ( (rc = (Func)) != SQLITE_OK  &&  rc != SQLITE_CONSTRAINT ) return rc

#endif
