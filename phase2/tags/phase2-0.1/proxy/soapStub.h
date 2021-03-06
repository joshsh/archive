/* soapStub.h
   Generated by gSOAP 2.7.1 from gSOAP_proxy.h
   Copyright (C) 2000-2005, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/
#ifndef soapStub_H
#define soapStub_H
#include "stdsoap2.h"
#ifdef __cplusplus
extern "C" {
#endif

SOAP_BEGIN_NAMESPACE(soap)

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


#ifndef _SOAP__Enum_1
#define _SOAP__Enum_1
enum _Enum_1 {false_ = 0, true_ = 1};
#endif

/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


/* Base64 schema type: */
struct xsd__base64Binary
{
	unsigned char *__ptr;
	int __size;
};

/* api:DirectoryCategory: */
struct api__DirectoryCategory
{
	char *fullViewableName;	/* optional element */
	char *specialEncoding;	/* optional element */
};

/* SOAP encoded array of api:DirectoryCategory schema type: */
struct DirectoryCategoryArray
{
	struct api__DirectoryCategory *__ptr;
	int __size;
};

/* api:ResultElement: */
struct api__ResultElement
{
	char *summary;	/* optional element */
	char *URL;	/* optional element */
	char *snippet;	/* optional element */
	char *title;	/* optional element */
	char *cachedSize;	/* optional element */
	enum _Enum_1 relatedInformationPresent;	/* required element */
	char *hostName;	/* optional element */
	struct api__DirectoryCategory directoryCategory;	/* required element */
	char *directoryTitle;	/* optional element */
};

/* SOAP encoded array of api:ResultElement schema type: */
struct ResultElementArray
{
	struct api__ResultElement *__ptr;
	int __size;
};

/* api:GoogleSearchResult: */
struct api__GoogleSearchResult
{
	enum _Enum_1 documentFiltering;	/* required element */
	char *searchComments;	/* optional element */
	int estimatedTotalResultsCount;	/* required element */
	enum _Enum_1 estimateIsExact;	/* required element */
	struct ResultElementArray resultElements;	/* required element */
	char *searchQuery;	/* optional element */
	int startIndex;	/* required element */
	int endIndex;	/* required element */
	char *searchTips;	/* optional element */
	struct DirectoryCategoryArray directoryCategories;	/* required element */
	double searchTime;	/* required element */
};

/* api:doGoogleSearchResponse: */
struct api__doGoogleSearchResponse
{
	struct api__GoogleSearchResult _return;	/* RPC return element */	/* required element */
};

/* api:doGoogleSearch: */
struct api__doGoogleSearch
{
	char *key;	/* optional element */
	char *q;	/* optional element */
	int start;	/* required element */
	int maxResults;	/* required element */
	enum _Enum_1 filter;	/* required element */
	char *restrict_;	/* optional element */
	enum _Enum_1 safeSearch;	/* required element */
	char *lr;	/* optional element */
	char *ie;	/* optional element */
	char *oe;	/* optional element */
};

/* api:doGetCachedPageResponse: */
struct api__doGetCachedPageResponse
{
	struct xsd__base64Binary *_return;	/* RPC return element */	/* optional element */
};

/* api:doGetCachedPage: */
struct api__doGetCachedPage
{
	char *key;	/* optional element */
	char *url;	/* optional element */
};

/* api:doSpellingSuggestionResponse: */
struct api__doSpellingSuggestionResponse
{
	char **_return;	/* RPC return element */	/* optional element */
};

/* api:doSpellingSuggestion: */
struct api__doSpellingSuggestion
{
	char *key;	/* optional element */
	char *phrase;	/* optional element */
};

/* SOAP Header: */
struct SOAP_ENV__Header
{
	void *dummy;	/* transient */
};

/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
	char *SOAP_ENV__Value;	/* optional element */
	char *SOAP_ENV__Node;	/* optional element */
	char *SOAP_ENV__Role;	/* optional element */
};

/* SOAP-ENV:Detail: */
struct SOAP_ENV__Detail
{
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
	char *__any;
};

/* SOAP Fault: */
struct SOAP_ENV__Fault
{
	char *faultcode;	/* optional element */
	char *faultstring;	/* optional element */
	char *faultactor;	/* optional element */
	struct SOAP_ENV__Detail *detail;	/* optional element */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element */
	char *SOAP_ENV__Reason;	/* optional element */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element */
};

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

typedef char *_XML;
typedef char *_QName;
typedef char *xsd__string;
typedef enum _Enum_1 xsd__boolean;
typedef int xsd__int;
typedef double xsd__double;

/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Service Operations                                                         *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 api__doGoogleSearch(struct soap*, char *key, char *q, int start, int maxResults, enum _Enum_1 filter, char *restrict_, enum _Enum_1 safeSearch, char *lr, char *ie, char *oe, struct api__doGoogleSearchResponse *response);

SOAP_FMAC5 int SOAP_FMAC6 api__doGetCachedPage(struct soap*, char *key, char *url, struct xsd__base64Binary *_return);

SOAP_FMAC5 int SOAP_FMAC6 api__doSpellingSuggestion(struct soap*, char *key, char *phrase, char **_return);

/******************************************************************************\
 *                                                                            *
 * Stubs                                                                      *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 soap_call_api__doGoogleSearch(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *key, char *q, int start, int maxResults, enum _Enum_1 filter, char *restrict_, enum _Enum_1 safeSearch, char *lr, char *ie, char *oe, struct api__doGoogleSearchResponse *response);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_api__doGetCachedPage(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *key, char *url, struct xsd__base64Binary *_return);

SOAP_FMAC5 int SOAP_FMAC6 soap_call_api__doSpellingSuggestion(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *key, char *phrase, char **_return);

/******************************************************************************\
 *                                                                            *
 * Skeletons                                                                  *
 *                                                                            *
\******************************************************************************/

SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_api__doGoogleSearch(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_api__doGetCachedPage(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_api__doSpellingSuggestion(struct soap*);

SOAP_END_NAMESPACE(soap)

#ifdef __cplusplus
}
#endif

#endif

/* End of soapStub.h */
