
#ifndef __RC_2_H__
#define __RC_2_H__


#ifdef __cplusplus

extern "C" {

#endif


void rc2_keyschedule( unsigned short xkey[64],
                     const unsigned char *key,
                     unsigned len,
                     unsigned bits );

void rc2_encrypt( const unsigned short xkey[64],
                 const unsigned char *plain,
                 unsigned char *cipher );

void rc2_decrypt( const unsigned short xkey[64],
                 unsigned char *plain,
                 const unsigned char *cipher );


#ifdef __cplusplus
}
#endif


#endif