/* soapServer.c
   Generated by gSOAP 2.7.1 from gSOAP_proxy.h
   Copyright (C) 2000-2005, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/
#include "soapH.h"
#ifdef __cplusplus
extern "C" {
#endif

SOAP_BEGIN_NAMESPACE(soap)

SOAP_SOURCE_STAMP("@(#) soapServer.c ver 2.7.1 2005-04-26 04:10:44 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap *soap)
{
	unsigned int n = soap->max_keep_alive;
	do
	{	soap_begin(soap);
		if (!--n)
			soap->keep_alive = 0;
		if (soap_begin_recv(soap))
		{	if (soap->error < SOAP_STOP)
				return soap_send_fault(soap);
			soap_closesock(soap);
			continue;
		}
		if (soap_envelope_begin_in(soap)
		 || soap_recv_header(soap)
		 || soap_body_begin_in(soap)
		 || soap_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap)))
			return soap_send_fault(soap);
		
	} while (soap->keep_alive);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap *soap)
{
	soap_peek_element(soap);
	if (!soap_match_tag(soap, soap->tag, "api:doGoogleSearch"))
		return soap_serve_api__doGoogleSearch(soap);
	if (!soap_match_tag(soap, soap->tag, "api:doGetCachedPage"))
		return soap_serve_api__doGetCachedPage(soap);
	if (!soap_match_tag(soap, soap->tag, "api:doSpellingSuggestion"))
		return soap_serve_api__doSpellingSuggestion(soap);
	return soap->error = SOAP_NO_METHOD;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_api__doGoogleSearch(struct soap *soap)
{	struct api__doGoogleSearch soap_tmp_api__doGoogleSearch;
	struct api__doGoogleSearchResponse response;
	soap_default_api__doGoogleSearchResponse(soap, &response);
	soap_default_api__doGoogleSearch(soap, &soap_tmp_api__doGoogleSearch);
	soap->encodingStyle = "";
	if (!soap_get_api__doGoogleSearch(soap, &soap_tmp_api__doGoogleSearch, "api:doGoogleSearch", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
#ifndef WITH_LEANER
	 || soap_resolve_attachments(soap)
#endif
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = api__doGoogleSearch(soap, soap_tmp_api__doGoogleSearch.key, soap_tmp_api__doGoogleSearch.q, soap_tmp_api__doGoogleSearch.start, soap_tmp_api__doGoogleSearch.maxResults, soap_tmp_api__doGoogleSearch.filter, soap_tmp_api__doGoogleSearch.restrict_, soap_tmp_api__doGoogleSearch.safeSearch, soap_tmp_api__doGoogleSearch.lr, soap_tmp_api__doGoogleSearch.ie, soap_tmp_api__doGoogleSearch.oe, &response);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_api__doGoogleSearchResponse(soap, &response);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_api__doGoogleSearchResponse(soap, &response, "api:doGoogleSearchResponse", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	};
	if (soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_api__doGoogleSearchResponse(soap, &response, "api:doGoogleSearchResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_api__doGetCachedPage(struct soap *soap)
{	struct api__doGetCachedPage soap_tmp_api__doGetCachedPage;
	struct api__doGetCachedPageResponse soap_tmp_api__doGetCachedPageResponse;
	struct xsd__base64Binary soap_tmp_xsd__base64Binary;
	soap_default_api__doGetCachedPageResponse(soap, &soap_tmp_api__doGetCachedPageResponse);
	soap_default_xsd__base64Binary(soap, &soap_tmp_xsd__base64Binary);
	soap_tmp_api__doGetCachedPageResponse._return = &soap_tmp_xsd__base64Binary;
	soap_default_api__doGetCachedPage(soap, &soap_tmp_api__doGetCachedPage);
	soap->encodingStyle = "";
	if (!soap_get_api__doGetCachedPage(soap, &soap_tmp_api__doGetCachedPage, "api:doGetCachedPage", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
#ifndef WITH_LEANER
	 || soap_resolve_attachments(soap)
#endif
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = api__doGetCachedPage(soap, soap_tmp_api__doGetCachedPage.key, soap_tmp_api__doGetCachedPage.url, &soap_tmp_xsd__base64Binary);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_api__doGetCachedPageResponse(soap, &soap_tmp_api__doGetCachedPageResponse);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_api__doGetCachedPageResponse(soap, &soap_tmp_api__doGetCachedPageResponse, "api:doGetCachedPageResponse", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	};
	if (soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_api__doGetCachedPageResponse(soap, &soap_tmp_api__doGetCachedPageResponse, "api:doGetCachedPageResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_api__doSpellingSuggestion(struct soap *soap)
{	struct api__doSpellingSuggestion soap_tmp_api__doSpellingSuggestion;
	struct api__doSpellingSuggestionResponse soap_tmp_api__doSpellingSuggestionResponse;
	char * soap_tmp_xsd__string;
	soap_default_api__doSpellingSuggestionResponse(soap, &soap_tmp_api__doSpellingSuggestionResponse);
	soap_tmp_xsd__string = NULL;
	soap_tmp_api__doSpellingSuggestionResponse._return = &soap_tmp_xsd__string;
	soap_default_api__doSpellingSuggestion(soap, &soap_tmp_api__doSpellingSuggestion);
	soap->encodingStyle = "";
	if (!soap_get_api__doSpellingSuggestion(soap, &soap_tmp_api__doSpellingSuggestion, "api:doSpellingSuggestion", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
#ifndef WITH_LEANER
	 || soap_resolve_attachments(soap)
#endif
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = api__doSpellingSuggestion(soap, soap_tmp_api__doSpellingSuggestion.key, soap_tmp_api__doSpellingSuggestion.phrase, &soap_tmp_xsd__string);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_api__doSpellingSuggestionResponse(soap, &soap_tmp_api__doSpellingSuggestionResponse);
	soap_begin_count(soap);
	if (soap->mode & SOAP_IO_LENGTH)
	{	soap_envelope_begin_out(soap);
		soap_putheader(soap);
		soap_body_begin_out(soap);
		soap_put_api__doSpellingSuggestionResponse(soap, &soap_tmp_api__doSpellingSuggestionResponse, "api:doSpellingSuggestionResponse", "");
		soap_body_end_out(soap);
		soap_envelope_end_out(soap);
	};
	if (soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_api__doSpellingSuggestionResponse(soap, &soap_tmp_api__doSpellingSuggestionResponse, "api:doSpellingSuggestionResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_END_NAMESPACE(soap)

#ifdef __cplusplus
}
#endif

/* End of soapServer.c */
