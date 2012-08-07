#ifndef PARCEL_H
#define PARCEL_H

#include <Windows.h>
typedef DWORD typeParcelSize;

/**
 *	@brief
 *	A <pre>Parcel</pre> represents a network-ready chunk of data to be sent between client and 
 *	server applications.  <pre>Parcel</pre>s are contain raw <pre>Message</pre> data and metadata
 *	to facilitate the unpacking of this data from the <pre>Parcel</pre> recipient.
 *	
 *	A <pre>Parcel</pre> contains four components: The size of the <pre>Parcel</pre> (<pre>size</pre>), in bytes; the 
 *	unique class ID of <pre>Message</pre> whose raw data is contained by the <pre>Parcel</pre> (<pre>message_type</pre>);
 *	the size of the raw </pre>Message</pre> data, in bytes (<pre>message_size</pre>); and the raw </pre>Message</pre>
 *	data itself (<pre>message_buff</pre>).
 */
struct Parcel
{
	typeParcelSize size;
	int message_type;
	DWORD message_size;
	char* message_buff;
};

#endif //ifndef PARCEL_H