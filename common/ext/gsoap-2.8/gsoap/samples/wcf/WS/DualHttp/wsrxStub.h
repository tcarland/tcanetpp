/* wsrxStub.h
   Generated by gSOAP 2.8.12 from ../../../../import/wsrm5.h

Copyright(C) 2000-2012, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
1) GPL or 2) Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef wsrxStub_H
#define wsrxStub_H
#define SOAP_WSRM_2005
#define SOAP_WSA_2005
#define SOAP_NAMESPACE_OF_wsrm	"http://schemas.xmlsoap.org/ws/2005/02/rm"
#include "stdsoap2.h"
#if GSOAP_VERSION != 20812
# error "GSOAP VERSION MISMATCH IN GENERATED CODE: PLEASE REINSTALL PACKAGE"
#endif


/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


#ifndef SOAP_TYPE_wsa5__RelationshipType
#define SOAP_TYPE_wsa5__RelationshipType (14)
/* wsa5:RelationshipType */
enum wsa5__RelationshipType {http_x003a_x002f_x002fwww_x002ew3_x002eorg_x002f2005_x002f08_x002faddressing_x002freply = 0};
typedef enum wsa5__RelationshipType wsa5__RelationshipType;
#endif

#ifndef SOAP_TYPE_wsa5__FaultCodesType
#define SOAP_TYPE_wsa5__FaultCodesType (15)
/* wsa5:FaultCodesType */
enum wsa5__FaultCodesType {wsa5__InvalidAddressingHeader = 0, wsa5__InvalidAddress = 1, wsa5__InvalidEPR = 2, wsa5__InvalidCardinality = 3, wsa5__MissingAddressInEPR = 4, wsa5__DuplicateMessageID = 5, wsa5__ActionMismatch = 6, wsa5__MessageAddressingHeaderRequired = 7, wsa5__DestinationUnreachable = 8, wsa5__ActionNotSupported = 9, wsa5__EndpointUnavailable = 10};
typedef enum wsa5__FaultCodesType wsa5__FaultCodesType;
#endif

#ifndef SOAP_TYPE__wsa5__IsReferenceParameter
#define SOAP_TYPE__wsa5__IsReferenceParameter (35)
/* wsa5:IsReferenceParameter */
enum _wsa5__IsReferenceParameter {_wsa5__IsReferenceParameter__false = 0, _wsa5__IsReferenceParameter__true = 1};
typedef enum _wsa5__IsReferenceParameter _wsa5__IsReferenceParameter;
#endif

#ifndef SOAP_TYPE_wsrm__FaultCodes
#define SOAP_TYPE_wsrm__FaultCodes (71)
/* wsrm:FaultCodes */
enum wsrm__FaultCodes {wsrm__SequenceTerminated = 0, wsrm__UnknownSequence = 1, wsrm__InvalidAcknowledgement = 2, wsrm__MessageNumberRollover = 3, wsrm__LastMessageNumberExceeded = 4, wsrm__CreateSequenceRefused = 5, wsrm__WSRMRequired = 6};
typedef enum wsrm__FaultCodes wsrm__FaultCodes;
#endif

#ifndef SOAP_TYPE_wsrm__IncompleteSequenceBehaviorType
#define SOAP_TYPE_wsrm__IncompleteSequenceBehaviorType (72)
/* wsrm:IncompleteSequenceBehaviorType */
enum wsrm__IncompleteSequenceBehaviorType {DiscardEntireSequence = 0, DiscardFollowingFirstGap = 1, NoDiscard = 2};
typedef enum wsrm__IncompleteSequenceBehaviorType wsrm__IncompleteSequenceBehaviorType;
#endif

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE_xsd__duration
#define SOAP_TYPE_xsd__duration (68)
typedef LONG64 xsd__duration;
#endif

/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE_wsa5__EndpointReferenceType
#define SOAP_TYPE_wsa5__EndpointReferenceType (7)
/* wsa5:EndpointReferenceType */
struct wsa5__EndpointReferenceType
{
public:
	char *Address;	/* required element of type xsd:string */
	struct wsa5__ReferenceParametersType *ReferenceParameters;	/* optional element of type wsa5:ReferenceParametersType */
	struct wsa5__MetadataType *Metadata;	/* optional element of type wsa5:MetadataType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
public:
	int soap_type() const { return 7; } /* = unique id SOAP_TYPE_wsa5__EndpointReferenceType */
};
typedef struct wsa5__EndpointReferenceType wsa5__EndpointReferenceType;
#endif

#ifndef SOAP_TYPE_wsa5__ReferenceParametersType
#define SOAP_TYPE_wsa5__ReferenceParametersType (8)
/* wsa5:ReferenceParametersType */
struct wsa5__ReferenceParametersType
{
public:
	int *chan__ChannelInstance;	/* optional element of type xsd:int */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
public:
	int soap_type() const { return 8; } /* = unique id SOAP_TYPE_wsa5__ReferenceParametersType */
};
typedef struct wsa5__ReferenceParametersType wsa5__ReferenceParametersType;
#endif

#ifndef SOAP_TYPE_wsa5__MetadataType
#define SOAP_TYPE_wsa5__MetadataType (9)
/* wsa5:MetadataType */
struct wsa5__MetadataType
{
public:
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
public:
	int soap_type() const { return 9; } /* = unique id SOAP_TYPE_wsa5__MetadataType */
};
typedef struct wsa5__MetadataType wsa5__MetadataType;
#endif

#ifndef SOAP_TYPE_wsa5__ProblemActionType
#define SOAP_TYPE_wsa5__ProblemActionType (11)
/* wsa5:ProblemActionType */
struct wsa5__ProblemActionType
{
public:
	char *Action;	/* optional element of type xsd:string */
	char *SoapAction;	/* optional element of type xsd:string */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
public:
	int soap_type() const { return 11; } /* = unique id SOAP_TYPE_wsa5__ProblemActionType */
};
typedef struct wsa5__ProblemActionType wsa5__ProblemActionType;
#endif

#ifndef SOAP_TYPE_wsa5__RelatesToType
#define SOAP_TYPE_wsa5__RelatesToType (10)
/* Primitive wsa5:RelatesToType schema type: */
struct wsa5__RelatesToType
{
public:
	char *__item;
	char *RelationshipType;	/* optional attribute of type wsa5:RelationshipTypeOpenEnum */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
public:
	int soap_type() const { return 10; } /* = unique id SOAP_TYPE_wsa5__RelatesToType */
};
typedef struct wsa5__RelatesToType wsa5__RelatesToType;
#endif

#ifndef SOAP_TYPE_chan__ChannelInstanceType
#define SOAP_TYPE_chan__ChannelInstanceType (36)
/* Primitive chan:ChannelInstanceType schema type: */
struct chan__ChannelInstanceType
{
public:
	int __item;
	enum _wsa5__IsReferenceParameter wsa5__IsReferenceParameter;	/* optional attribute of type wsa5:IsReferenceParameter */
public:
	int soap_type() const { return 36; } /* = unique id SOAP_TYPE_chan__ChannelInstanceType */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (37)
/* SOAP Header: */
struct SOAP_ENV__Header
{
public:
	char *wsa5__MessageID;	/* optional element of type wsa5:MessageID */
	struct wsa5__RelatesToType *wsa5__RelatesTo;	/* optional element of type wsa5:RelatesTo */
	struct wsa5__EndpointReferenceType *wsa5__From;	/* optional element of type wsa5:From */
	struct wsa5__EndpointReferenceType *wsa5__ReplyTo;	/* mustUnderstand */
	struct wsa5__EndpointReferenceType *wsa5__FaultTo;	/* mustUnderstand */
	char *wsa5__To;	/* mustUnderstand */
	char *wsa5__Action;	/* mustUnderstand */
	struct chan__ChannelInstanceType *chan__ChannelInstance;	/* optional element of type chan:ChannelInstanceType */
	struct wsrm__SequenceType *wsrm__Sequence;	/* optional element of type wsrm:SequenceType */
	int __sizeAckRequested;	/* sequence of elements <wsrm:AckRequested> */
	struct wsrm__AckRequestedType *wsrm__AckRequested;	/* optional element of type wsrm:AckRequestedType */
	int __sizeSequenceAcknowledgement;	/* sequence of elements <wsrm:SequenceAcknowledgement> */
	struct _wsrm__SequenceAcknowledgement *wsrm__SequenceAcknowledgement;	/* optional element of type wsrm:SequenceAcknowledgement */
	struct wsrm__SequenceFaultType *wsrm__SequenceFault;	/* optional element of type wsrm:SequenceFaultType */
public:
	int soap_type() const { return 37; } /* = unique id SOAP_TYPE_SOAP_ENV__Header */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (43)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
public:
	int soap_type() const { return 43; } /* = unique id SOAP_TYPE_SOAP_ENV__Detail */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (45)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
public:
	int soap_type() const { return 45; } /* = unique id SOAP_TYPE_SOAP_ENV__Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (47)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
public:
	int soap_type() const { return 47; } /* = unique id SOAP_TYPE_SOAP_ENV__Reason */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (51)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
public:
	int soap_type() const { return 51; } /* = unique id SOAP_TYPE_SOAP_ENV__Fault */
};
#endif

#endif

#ifndef SOAP_TYPE_wsrm__SequenceType
#define SOAP_TYPE_wsrm__SequenceType (52)
/* wsrm:SequenceType */
struct wsrm__SequenceType
{
public:
	char *Identifier;	/* required element of type xsd:string */
	ULONG64 MessageNumber;	/* required element of type xsd:unsignedLong */
	struct _wsrm__UsesSequenceSSL *LastMessage;	/* optional element of type wsrm:UsesSequenceSSL */
public:
	int soap_type() const { return 52; } /* = unique id SOAP_TYPE_wsrm__SequenceType */
};
typedef struct wsrm__SequenceType wsrm__SequenceType;
#endif

#ifndef SOAP_TYPE_wsrm__AckRequestedType
#define SOAP_TYPE_wsrm__AckRequestedType (53)
/* wsrm:AckRequestedType */
struct wsrm__AckRequestedType
{
public:
	char *Identifier;	/* required element of type xsd:string */
public:
	int soap_type() const { return 53; } /* = unique id SOAP_TYPE_wsrm__AckRequestedType */
};
typedef struct wsrm__AckRequestedType wsrm__AckRequestedType;
#endif

#ifndef SOAP_TYPE_wsrm__SequenceFaultType
#define SOAP_TYPE_wsrm__SequenceFaultType (54)
/* wsrm:SequenceFaultType */
struct wsrm__SequenceFaultType
{
public:
	enum wsrm__FaultCodes FaultCode;	/* required element of type wsrm:FaultCodes */
	struct SOAP_ENV__Detail *Detail;	/* optional element of type SOAP-ENV:Detail */
public:
	int soap_type() const { return 54; } /* = unique id SOAP_TYPE_wsrm__SequenceFaultType */
};
typedef struct wsrm__SequenceFaultType wsrm__SequenceFaultType;
#endif

#ifndef SOAP_TYPE_wsrm__CreateSequenceType
#define SOAP_TYPE_wsrm__CreateSequenceType (55)
/* wsrm:CreateSequenceType */
struct wsrm__CreateSequenceType
{
public:
	struct wsa5__EndpointReferenceType AcksTo;	/* required element of type wsa5:EndpointReferenceType */
	LONG64 *Expires;	/* optional element of type xsd:duration */
	struct wsrm__OfferType *Offer;	/* optional element of type wsrm:OfferType */
public:
	int soap_type() const { return 55; } /* = unique id SOAP_TYPE_wsrm__CreateSequenceType */
};
typedef struct wsrm__CreateSequenceType wsrm__CreateSequenceType;
#endif

#ifndef SOAP_TYPE_wsrm__CreateSequenceResponseType
#define SOAP_TYPE_wsrm__CreateSequenceResponseType (56)
/* wsrm:CreateSequenceResponseType */
struct wsrm__CreateSequenceResponseType
{
public:
	char *Identifier;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
	LONG64 *Expires;	/* optional element of type xsd:duration */
	enum wsrm__IncompleteSequenceBehaviorType *IncompleteSequenceBehavior;	/* optional element of type wsrm:IncompleteSequenceBehaviorType */
	struct wsrm__AcceptType *Accept;	/* optional element of type wsrm:AcceptType */
public:
	int soap_type() const { return 56; } /* = unique id SOAP_TYPE_wsrm__CreateSequenceResponseType */
};
typedef struct wsrm__CreateSequenceResponseType wsrm__CreateSequenceResponseType;
#endif

#ifndef SOAP_TYPE_wsrm__CloseSequenceType
#define SOAP_TYPE_wsrm__CloseSequenceType (57)
/* wsrm:CloseSequenceType */
struct wsrm__CloseSequenceType
{
public:
	char *Identifier;	/* required element of type xsd:string */
	ULONG64 *LastMsgNumber;	/* optional element of type xsd:unsignedLong */
public:
	int soap_type() const { return 57; } /* = unique id SOAP_TYPE_wsrm__CloseSequenceType */
};
typedef struct wsrm__CloseSequenceType wsrm__CloseSequenceType;
#endif

#ifndef SOAP_TYPE_wsrm__CloseSequenceResponseType
#define SOAP_TYPE_wsrm__CloseSequenceResponseType (58)
/* wsrm:CloseSequenceResponseType */
struct wsrm__CloseSequenceResponseType
{
public:
	char *Identifier;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
public:
	int soap_type() const { return 58; } /* = unique id SOAP_TYPE_wsrm__CloseSequenceResponseType */
};
typedef struct wsrm__CloseSequenceResponseType wsrm__CloseSequenceResponseType;
#endif

#ifndef SOAP_TYPE_wsrm__TerminateSequenceType
#define SOAP_TYPE_wsrm__TerminateSequenceType (59)
/* wsrm:TerminateSequenceType */
struct wsrm__TerminateSequenceType
{
public:
	char *Identifier;	/* required element of type xsd:string */
	ULONG64 *LastMsgNumber;	/* optional element of type xsd:unsignedLong */
public:
	int soap_type() const { return 59; } /* = unique id SOAP_TYPE_wsrm__TerminateSequenceType */
};
typedef struct wsrm__TerminateSequenceType wsrm__TerminateSequenceType;
#endif

#ifndef SOAP_TYPE_wsrm__TerminateSequenceResponseType
#define SOAP_TYPE_wsrm__TerminateSequenceResponseType (60)
/* wsrm:TerminateSequenceResponseType */
struct wsrm__TerminateSequenceResponseType
{
public:
	char *Identifier;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
	ULONG64 *LastMsgNumber;	/* optional element of type xsd:unsignedLong */
public:
	int soap_type() const { return 60; } /* = unique id SOAP_TYPE_wsrm__TerminateSequenceResponseType */
};
typedef struct wsrm__TerminateSequenceResponseType wsrm__TerminateSequenceResponseType;
#endif

#ifndef SOAP_TYPE_wsrm__OfferType
#define SOAP_TYPE_wsrm__OfferType (61)
/* wsrm:OfferType */
struct wsrm__OfferType
{
public:
	char *Identifier;	/* required element of type xsd:string */
	struct wsa5__EndpointReferenceType Endpoint;	/* required element of type wsa5:EndpointReferenceType */
	LONG64 *Expires;	/* optional element of type xsd:duration */
	enum wsrm__IncompleteSequenceBehaviorType *IncompleteSequenceBehavior;	/* optional element of type wsrm:IncompleteSequenceBehaviorType */
public:
	int soap_type() const { return 61; } /* = unique id SOAP_TYPE_wsrm__OfferType */
};
typedef struct wsrm__OfferType wsrm__OfferType;
#endif

#ifndef SOAP_TYPE_wsrm__AcceptType
#define SOAP_TYPE_wsrm__AcceptType (62)
/* wsrm:AcceptType */
struct wsrm__AcceptType
{
public:
	struct wsa5__EndpointReferenceType AcksTo;	/* required element of type wsa5:EndpointReferenceType */
public:
	int soap_type() const { return 62; } /* = unique id SOAP_TYPE_wsrm__AcceptType */
};
typedef struct wsrm__AcceptType wsrm__AcceptType;
#endif

#ifndef SOAP_TYPE__wsrm__SequenceAcknowledgement_Final
#define SOAP_TYPE__wsrm__SequenceAcknowledgement_Final (79)
/* wsrm:SequenceAcknowledgement-Final */
struct _wsrm__SequenceAcknowledgement_Final
{
public:
	int soap_type() const { return 79; } /* = unique id SOAP_TYPE__wsrm__SequenceAcknowledgement_Final */
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__wsrm__SequenceAcknowledgement_AcknowledgementRange
#define SOAP_TYPE__wsrm__SequenceAcknowledgement_AcknowledgementRange (81)
/* wsrm:SequenceAcknowledgement-AcknowledgementRange */
struct _wsrm__SequenceAcknowledgement_AcknowledgementRange
{
public:
	ULONG64 Upper;	/* required attribute of type xsd:unsignedLong */
	ULONG64 Lower;	/* required attribute of type xsd:unsignedLong */
public:
	int soap_type() const { return 81; } /* = unique id SOAP_TYPE__wsrm__SequenceAcknowledgement_AcknowledgementRange */
};
#endif

#ifndef SOAP_TYPE__wsrm__SequenceAcknowledgement_None
#define SOAP_TYPE__wsrm__SequenceAcknowledgement_None (83)
/* wsrm:SequenceAcknowledgement-None */
struct _wsrm__SequenceAcknowledgement_None
{
public:
	int soap_type() const { return 83; } /* = unique id SOAP_TYPE__wsrm__SequenceAcknowledgement_None */
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__wsrm__SequenceAcknowledgement
#define SOAP_TYPE__wsrm__SequenceAcknowledgement (63)
/* wsrm:SequenceAcknowledgement */
struct _wsrm__SequenceAcknowledgement
{
public:
	char *Identifier;	/* required element of type xsd:string */
	int __sizeNack;	/* sequence of elements <Nack> */
	ULONG64 *Nack;	/* optional element of type xsd:unsignedLong */
	struct _wsrm__SequenceAcknowledgement_Final *Final;	/* optional element of type wsrm:SequenceAcknowledgement-Final */
	int __sizeAcknowledgementRange;	/* sequence of elements <AcknowledgementRange> */
	struct _wsrm__SequenceAcknowledgement_AcknowledgementRange *AcknowledgementRange;	/* optional element of type wsrm:SequenceAcknowledgement-AcknowledgementRange */
	struct _wsrm__SequenceAcknowledgement_None *None;	/* optional element of type wsrm:SequenceAcknowledgement-None */
	char *__any;
public:
	int soap_type() const { return 63; } /* = unique id SOAP_TYPE__wsrm__SequenceAcknowledgement */
};
typedef struct _wsrm__SequenceAcknowledgement _wsrm__SequenceAcknowledgement;
#endif

#ifndef SOAP_TYPE__wsrm__UsesSequenceSTR
#define SOAP_TYPE__wsrm__UsesSequenceSTR (69)
/* wsrm:UsesSequenceSTR */
struct _wsrm__UsesSequenceSTR
{
public:
	int soap_type() const { return 69; } /* = unique id SOAP_TYPE__wsrm__UsesSequenceSTR */
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
typedef struct _wsrm__UsesSequenceSTR _wsrm__UsesSequenceSTR;
#endif

#ifndef SOAP_TYPE__wsrm__UsesSequenceSSL
#define SOAP_TYPE__wsrm__UsesSequenceSSL (70)
/* wsrm:UsesSequenceSSL */
struct _wsrm__UsesSequenceSSL
{
public:
	int soap_type() const { return 70; } /* = unique id SOAP_TYPE__wsrm__UsesSequenceSSL */
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
typedef struct _wsrm__UsesSequenceSSL _wsrm__UsesSequenceSSL;
#endif

#ifndef SOAP_TYPE___wsrm__CreateSequence
#define SOAP_TYPE___wsrm__CreateSequence (92)
/* Operation wrapper: */
struct __wsrm__CreateSequence
{
public:
	struct wsrm__CreateSequenceType *wsrm__CreateSequence;	/* optional element of type wsrm:CreateSequenceType */
public:
	int soap_type() const { return 92; } /* = unique id SOAP_TYPE___wsrm__CreateSequence */
};
#endif

#ifndef SOAP_TYPE___wsrm__CloseSequence
#define SOAP_TYPE___wsrm__CloseSequence (96)
/* Operation wrapper: */
struct __wsrm__CloseSequence
{
public:
	struct wsrm__CloseSequenceType *wsrm__CloseSequence;	/* optional element of type wsrm:CloseSequenceType */
public:
	int soap_type() const { return 96; } /* = unique id SOAP_TYPE___wsrm__CloseSequence */
};
#endif

#ifndef SOAP_TYPE___wsrm__TerminateSequence
#define SOAP_TYPE___wsrm__TerminateSequence (100)
/* Operation wrapper: */
struct __wsrm__TerminateSequence
{
public:
	struct wsrm__TerminateSequenceType *wsrm__TerminateSequence;	/* optional element of type wsrm:TerminateSequenceType */
public:
	int soap_type() const { return 100; } /* = unique id SOAP_TYPE___wsrm__TerminateSequence */
};
#endif

#ifndef SOAP_TYPE___wsrm__CreateSequenceResponse
#define SOAP_TYPE___wsrm__CreateSequenceResponse (102)
/* Operation wrapper: */
struct __wsrm__CreateSequenceResponse
{
public:
	struct wsrm__CreateSequenceResponseType *wsrm__CreateSequenceResponse;	/* optional element of type wsrm:CreateSequenceResponseType */
public:
	int soap_type() const { return 102; } /* = unique id SOAP_TYPE___wsrm__CreateSequenceResponse */
};
#endif

#ifndef SOAP_TYPE___wsrm__CloseSequenceResponse
#define SOAP_TYPE___wsrm__CloseSequenceResponse (104)
/* Operation wrapper: */
struct __wsrm__CloseSequenceResponse
{
public:
	struct wsrm__CloseSequenceResponseType *wsrm__CloseSequenceResponse;	/* optional element of type wsrm:CloseSequenceResponseType */
public:
	int soap_type() const { return 104; } /* = unique id SOAP_TYPE___wsrm__CloseSequenceResponse */
};
#endif

#ifndef SOAP_TYPE___wsrm__TerminateSequenceResponse
#define SOAP_TYPE___wsrm__TerminateSequenceResponse (106)
/* Operation wrapper: */
struct __wsrm__TerminateSequenceResponse
{
public:
	struct wsrm__TerminateSequenceResponseType *wsrm__TerminateSequenceResponse;	/* optional element of type wsrm:TerminateSequenceResponseType */
public:
	int soap_type() const { return 106; } /* = unique id SOAP_TYPE___wsrm__TerminateSequenceResponse */
};
#endif

#ifndef SOAP_TYPE___wsrm__SequenceAcknowledgement
#define SOAP_TYPE___wsrm__SequenceAcknowledgement (108)
/* Operation wrapper: */
struct __wsrm__SequenceAcknowledgement
{
public:
	int soap_type() const { return 108; } /* = unique id SOAP_TYPE___wsrm__SequenceAcknowledgement */
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___wsrm__AckRequested
#define SOAP_TYPE___wsrm__AckRequested (110)
/* Operation wrapper: */
struct __wsrm__AckRequested
{
public:
	int soap_type() const { return 110; } /* = unique id SOAP_TYPE___wsrm__AckRequested */
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___wsrm__LastMessage
#define SOAP_TYPE___wsrm__LastMessage (112)
/* Operation wrapper: */
struct __wsrm__LastMessage
{
public:
	int soap_type() const { return 112; } /* = unique id SOAP_TYPE___wsrm__LastMessage */
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif

#ifndef SOAP_TYPE_wsa5__RelationshipTypeOpenEnum
#define SOAP_TYPE_wsa5__RelationshipTypeOpenEnum (12)
typedef char *wsa5__RelationshipTypeOpenEnum;
#endif

#ifndef SOAP_TYPE_wsa5__FaultCodesOpenEnumType
#define SOAP_TYPE_wsa5__FaultCodesOpenEnumType (13)
typedef char *wsa5__FaultCodesOpenEnumType;
#endif

#ifndef SOAP_TYPE__wsa5__EndpointReference
#define SOAP_TYPE__wsa5__EndpointReference (20)
typedef struct wsa5__EndpointReferenceType _wsa5__EndpointReference;
#endif

#ifndef SOAP_TYPE__wsa5__ReferenceParameters
#define SOAP_TYPE__wsa5__ReferenceParameters (21)
typedef struct wsa5__ReferenceParametersType _wsa5__ReferenceParameters;
#endif

#ifndef SOAP_TYPE__wsa5__Metadata
#define SOAP_TYPE__wsa5__Metadata (22)
typedef struct wsa5__MetadataType _wsa5__Metadata;
#endif

#ifndef SOAP_TYPE__wsa5__MessageID
#define SOAP_TYPE__wsa5__MessageID (23)
typedef char *_wsa5__MessageID;
#endif

#ifndef SOAP_TYPE__wsa5__RelatesTo
#define SOAP_TYPE__wsa5__RelatesTo (24)
typedef struct wsa5__RelatesToType _wsa5__RelatesTo;
#endif

#ifndef SOAP_TYPE__wsa5__ReplyTo
#define SOAP_TYPE__wsa5__ReplyTo (25)
typedef struct wsa5__EndpointReferenceType _wsa5__ReplyTo;
#endif

#ifndef SOAP_TYPE__wsa5__From
#define SOAP_TYPE__wsa5__From (26)
typedef struct wsa5__EndpointReferenceType _wsa5__From;
#endif

#ifndef SOAP_TYPE__wsa5__FaultTo
#define SOAP_TYPE__wsa5__FaultTo (27)
typedef struct wsa5__EndpointReferenceType _wsa5__FaultTo;
#endif

#ifndef SOAP_TYPE__wsa5__To
#define SOAP_TYPE__wsa5__To (28)
typedef char *_wsa5__To;
#endif

#ifndef SOAP_TYPE__wsa5__Action
#define SOAP_TYPE__wsa5__Action (29)
typedef char *_wsa5__Action;
#endif

#ifndef SOAP_TYPE__wsa5__RetryAfter
#define SOAP_TYPE__wsa5__RetryAfter (31)
typedef ULONG64 _wsa5__RetryAfter;
#endif

#ifndef SOAP_TYPE__wsa5__ProblemHeaderQName
#define SOAP_TYPE__wsa5__ProblemHeaderQName (32)
typedef char *_wsa5__ProblemHeaderQName;
#endif

#ifndef SOAP_TYPE__wsa5__ProblemIRI
#define SOAP_TYPE__wsa5__ProblemIRI (33)
typedef char *_wsa5__ProblemIRI;
#endif

#ifndef SOAP_TYPE__wsa5__ProblemAction
#define SOAP_TYPE__wsa5__ProblemAction (34)
typedef struct wsa5__ProblemActionType _wsa5__ProblemAction;
#endif

#ifndef SOAP_TYPE__wsrm__Identifier
#define SOAP_TYPE__wsrm__Identifier (64)
typedef char *_wsrm__Identifier;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault(struct soap*, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__CreateSequence(struct soap*, struct wsrm__CreateSequenceType *wsrm__CreateSequence, struct wsrm__CreateSequenceResponseType *wsrm__CreateSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__CloseSequence(struct soap*, struct wsrm__CloseSequenceType *wsrm__CloseSequence, struct wsrm__CloseSequenceResponseType *wsrm__CloseSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__TerminateSequence(struct soap*, struct wsrm__TerminateSequenceType *wsrm__TerminateSequence, struct wsrm__TerminateSequenceResponseType *wsrm__TerminateSequence_);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__CreateSequenceResponse(struct soap*, struct wsrm__CreateSequenceResponseType *wsrm__CreateSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__CloseSequenceResponse(struct soap*, struct wsrm__CloseSequenceResponseType *wsrm__CloseSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__TerminateSequenceResponse(struct soap*, struct wsrm__TerminateSequenceResponseType *wsrm__TerminateSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__SequenceAcknowledgement(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__AckRequested(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 __wsrm__LastMessage(struct soap*);

/******************************************************************************\
 *                                                                            *
 * Server-Side Skeletons to Invoke Service Operations                         *
 *                                                                            *
\******************************************************************************/

extern "C" SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap*);

extern "C" SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_SOAP_ENV__Fault(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__CreateSequence(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__CloseSequence(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__TerminateSequence(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__CreateSequenceResponse(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__CloseSequenceResponse(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__TerminateSequenceResponse(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__SequenceAcknowledgement(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__AckRequested(struct soap*);

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsrm__LastMessage(struct soap*);

/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stubs                                                     *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 soap_send_SOAP_ENV__Fault(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv_SOAP_ENV__Fault(struct soap *soap, struct SOAP_ENV__Fault *_param_1);


SOAP_FMAC5 int SOAP_FMAC6 soap_call___wsrm__CreateSequence(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsrm__CreateSequenceType *wsrm__CreateSequence, struct wsrm__CreateSequenceResponseType *wsrm__CreateSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___wsrm__CloseSequence(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsrm__CloseSequenceType *wsrm__CloseSequence, struct wsrm__CloseSequenceResponseType *wsrm__CloseSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___wsrm__TerminateSequence(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsrm__TerminateSequenceType *wsrm__TerminateSequence, struct wsrm__TerminateSequenceResponseType *wsrm__TerminateSequence_);

SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsrm__CreateSequenceResponse(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsrm__CreateSequenceResponseType *wsrm__CreateSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsrm__CreateSequenceResponse(struct soap *soap, struct __wsrm__CreateSequenceResponse *_param_2);


SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsrm__CloseSequenceResponse(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsrm__CloseSequenceResponseType *wsrm__CloseSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsrm__CloseSequenceResponse(struct soap *soap, struct __wsrm__CloseSequenceResponse *_param_3);


SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsrm__TerminateSequenceResponse(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsrm__TerminateSequenceResponseType *wsrm__TerminateSequenceResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsrm__TerminateSequenceResponse(struct soap *soap, struct __wsrm__TerminateSequenceResponse *_param_4);


SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsrm__SequenceAcknowledgement(struct soap *soap, const char *soap_endpoint, const char *soap_action);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsrm__SequenceAcknowledgement(struct soap *soap, struct __wsrm__SequenceAcknowledgement *_param_5);


SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsrm__AckRequested(struct soap *soap, const char *soap_endpoint, const char *soap_action);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsrm__AckRequested(struct soap *soap, struct __wsrm__AckRequested *_param_6);


SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsrm__LastMessage(struct soap *soap, const char *soap_endpoint, const char *soap_action);

SOAP_FMAC5 int SOAP_FMAC6 soap_recv___wsrm__LastMessage(struct soap *soap, struct __wsrm__LastMessage *_param_7);


#endif

/* End of wsrxStub.h */