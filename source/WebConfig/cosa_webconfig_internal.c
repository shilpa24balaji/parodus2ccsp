/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_webconfig_apis.h"
#include "cosa_webconfig_dml.h"
#include "cosa_webconfig_internal.h"
#include "webconfig_log.h"
#include <webcfg_db.h>

#define WEBCONFIG_PARAM_RFC_ENABLE          "Device.X_RDK_WebConfig.RfcEnable"
#define WEBCONFIG_PARAM_URL                 "Device.X_RDK_WebConfig.URL"
#define WEBCONFIG_PARAM_FORCE_SYNC   	    "Device.X_RDK_WebConfig.ForceSync"

static char *paramRFCEnable = "eRT.com.cisco.spvtg.ccsp.webpa.Device.X_RDK_WebConfig.RfcEnable";

extern PCOSA_BACKEND_MANAGER_OBJECT g_pCosaBEManager;
extern ANSC_HANDLE bus_handle;
extern char        g_Subsystem[32];

BOOL Get_RfcEnable()
{
    PCOSA_DATAMODEL_WEBCONFIG            pMyObject           = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
    return pMyObject->RfcEnable;
}

int setRfcEnable(BOOL bValue)
{
	PCOSA_DATAMODEL_WEBCONFIG            pMyObject           = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	char buf[16] = {0};
	int retPsmSet = CCSP_SUCCESS;

	if(bValue == TRUE)
	{
		sprintf(buf, "%s", "true");
		WebcfgDebug("Received RFC enable. updating g_shutdown\n");
		if(pMyObject->RfcEnable == false)
		{
			//pthread_mutex_lock (get_global_periodicsync_mutex());
			//g_shutdown  = false;
			//pthread_mutex_unlock(get_global_periodicsync_mutex());
			WebConfigLog("RfcEnable dynamic change from false to true. start initWebConfigMultipartTask.\n");
			initWebConfigMultipartTask();
		}
	}
	else
	{
		sprintf(buf, "%s", "false");
		WebConfigLog("Received RFC disable. updating g_shutdown\n");
		/* sending signal to kill initWebConfigMultipartTask thread*/
		//pthread_mutex_lock (get_global_periodicsync_mutex());
		//g_shutdown  = true;
		//pthread_cond_signal(get_global_periodicsync_condition());
		//pthread_mutex_unlock(get_global_periodicsync_mutex());
	}  
#ifdef RDKB_BUILD
	retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, paramRFCEnable, ccsp_string, buf);
        if (retPsmSet != CCSP_SUCCESS)
        {
                WebConfigLog("psm_set failed ret %d for parameter %s and value %s\n", retPsmSet, paramRFCEnable, buf);
                return 1;
        }
        else
        {
                WebConfigLog("psm_set success ret %d for parameter %s and value %s\n", retPsmSet, paramRFCEnable, buf);
		pMyObject->RfcEnable = bValue;
		WebConfigLog("pMyObject->RfcEnable is %d\n", pMyObject->RfcEnable);
		return 0;
        }
#endif
	return 0;
}

BOOL CosaDmlGetRFCEnableFromDB(BOOL *pbValue)
{
    char* strValue = NULL;

    WebcfgDebug("-------- %s ----- Enter-- ---\n",__FUNCTION__);

    *pbValue = FALSE;
    if (CCSP_SUCCESS == PSM_Get_Record_Value2(bus_handle,
                g_Subsystem, paramRFCEnable, NULL, &strValue))
    {
        if(((strcmp (strValue, "true") == 0)) || (strcmp (strValue, "TRUE") == 0))
	{
            *pbValue = TRUE;
        }
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc( strValue );
        return TRUE;
    }
    WebcfgDebug("-------- %s ----- Exit-- ---\n",__FUNCTION__);
    return FALSE;
}

int Get_Webconfig_URL( char *pString)
{
    PCOSA_DATAMODEL_WEBCONFIG            pMyObject           = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
    WebConfigLog("-------- %s ----- Enter-- ---\n",__FUNCTION__);

        if((pMyObject != NULL) && (pMyObject->URL != NULL) && (strlen(pMyObject->URL)>0))
        {
        		WebConfigLog("pMyObject->URL %s\n", pMyObject->URL);
                WebConfigLog("%s ----- updating pString ------\n",__FUNCTION__);
		
		AnscCopyString( pString,pMyObject->URL );
		WebConfigLog("pString %s\n",pString);
        }
        else
        {
		char *tempDBUrl = NULL;
                int   retPsmGet    = CCSP_SUCCESS;
		WebConfigLog("B4 PSM_Get_Record_Value2\n");
                retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, WEBCONFIG_PARAM_URL, NULL, &tempDBUrl);
		WebConfigLog("After PSM_Get_Record_Value2\n");
		WebConfigLog("tempDBUrl is %s\n", tempDBUrl);
                if (retPsmGet == CCSP_SUCCESS)
                {
			WebConfigLog("retPsmGet success\n");
			AnscCopyString( pString,tempDBUrl);
                        WebConfigLog("pString %s\n",pString);
                }
                else
                {
                        WebConfigLog("psm_get failed ret %d for parameter %s\n", retPsmGet, WEBCONFIG_PARAM_URL);
                        return 0;
                }
        }

        WebConfigLog("-------- %s ----- Exit ------\n",__FUNCTION__);
        return 1;
}

int Set_Webconfig_URL( char *pString)
{
    PCOSA_DATAMODEL_WEBCONFIG            pMyObject           = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
    WebConfigLog("-------- %s ----- Enter ------\n",__FUNCTION__);
    int retPsmSet = CCSP_SUCCESS;

        memset( pMyObject->URL, 0, sizeof( pMyObject->URL ));
        AnscCopyString( pMyObject->URL, pString );


        retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, WEBCONFIG_PARAM_URL, ccsp_string, pString);
        if (retPsmSet != CCSP_SUCCESS)
        {
                WebConfigLog("psm_set failed ret %d for parameter %s and value %s\n", retPsmSet, WEBCONFIG_PARAM_URL, pString);
                return 0;
        }
        else
        {
                WebConfigLog("psm_set success ret %d for parameter %s and value %s\n", retPsmSet, WEBCONFIG_PARAM_URL, pString);
        }

        WebConfigLog("-------- %s ----- Exit ------\n",__FUNCTION__);
        return 1;
}

int Get_Webconfig_Blob( char *pString)
{
    PCOSA_DATAMODEL_WEBCONFIG            pMyObject           = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
    WebConfigLog("-------- %s ----- Enter-- ---\n",__FUNCTION__);

	//WebConfigLog("pMyObject->DBBlobData %s,*pString=%s\n", pMyObject->DBBlobData,*pString);

        pMyObject->DBBlobData = NULL;
        pMyObject->DBBlobData = get_DB_BLOB_base64(&pMyObject->DBBlobLength);
        writeBlobToFile(WEBCFG_BLOB_PATH, pMyObject->DBBlobData);
        WebConfigLog("pMyObject->DBBlobLength %zu\n",pMyObject->DBBlobLength);
        readBlobFromFile(WEBCFG_BLOB_PATH);
	pString=(char*) malloc(sizeof(char *)*(pMyObject->DBBlobLength));
        AnscCopyString( pString,pMyObject->DBBlobData );
        if (pString != NULL)
        {
            WebConfigLog("pMyObject->DBBlobLength %zu, pString %s, pMyObject->DBBlobData=%s\n",pMyObject->DBBlobLength,pString,pMyObject->DBBlobData);
            return 1;
        }
        else
        {
            WebConfigLog("Failed to get b64 encoded data\n");
            return 0;
        }
}


int setForceSync(char* pString, char *transactionId,int *pStatus)
{
	PCOSA_DATAMODEL_WEBCONFIG            pMyObject           = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	WebConfigLog("setForceSync\n");
	memset( pMyObject->ForceSync, 0, sizeof( pMyObject->ForceSync ));
	AnscCopyString( pMyObject->ForceSync, pString );
	WebConfigLog("pMyObject->ForceSync is %s\n", pMyObject->ForceSync);

	if((pMyObject->ForceSync !=NULL) && (strlen(pMyObject->ForceSync)>0))
	{
		if(strlen(pMyObject->ForceSyncTransID)>0)
		{
			WebConfigLog("Force sync is already in progress, Ignoring this request.\n");
			*pStatus = 1;
			return 0;
		}
		else
		{
			/* sending signal to initWebConfigMultipartTask to update the sync time interval*/
			//pthread_mutex_lock (get_global_periodicsync_mutex());

			//Update ForceSyncTransID to access webpa transactionId in webConfig sync.
			if(transactionId !=NULL && (strlen(transactionId)>0))
			{
				AnscCopyString(pMyObject->ForceSyncTransID, transactionId);
				WebConfigLog("pMyObject->ForceSyncTransID is %s\n", pMyObject->ForceSyncTransID);
			}
			WebConfigLog("Trigger force sync\n");
			//pthread_cond_signal(get_global_periodicsync_condition());
			//pthread_mutex_unlock(get_global_periodicsync_mutex());
		}
	}
	else
	{
		WebConfigLog("Force sync param set with empty value\n");
		memset(pMyObject->ForceSyncTransID,0,sizeof(pMyObject->ForceSyncTransID));
	}
	WebConfigLog("setForceSync returns success 1\n");
	return 1;
}

int getForceSync(char** pString, char **transactionId )
{
	PCOSA_DATAMODEL_WEBCONFIG pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	WebConfigLog("-------- %s ----- Enter ------\n",__FUNCTION__);

	if((pMyObject->ForceSync != NULL) && strlen(pMyObject->ForceSync)>0)
	{
		WebConfigLog("%s ----- updating pString ------\n",__FUNCTION__);
		*pString = strdup(pMyObject->ForceSync);
		WebConfigLog("%s ----- updating transactionId ------\n",__FUNCTION__);
		*transactionId = strdup(pMyObject->ForceSyncTransID);
	}
	else
	{
		*pString = NULL;
		*transactionId = NULL;
		return 0;
	}
	WebConfigLog("*transactionId is %s\n",*transactionId);
	WebConfigLog("-------- %s ----- Exit ------\n",__FUNCTION__);
	return 1;
}

int getWebConfigParameterValues(char **parameterNames, int paramCount, int *val_size, parameterValStruct_t ***val)
{
    char *objects[] ={"Device.X_RDK_WebConfig."};
    int objSize = sizeof(objects)/sizeof(objects[0]);
    parameterValStruct_t **paramVal = NULL;
    paramVal = (parameterValStruct_t **) malloc(sizeof(parameterValStruct_t *)*paramCount);
    int i=0, j=0, k=0, isWildcard = 0, matchFound = 0;
    int localCount = paramCount;
    BOOL RFC_ENABLE;
    WebcfgDebug("*********** %s ***************\n",__FUNCTION__);

    RFC_ENABLE = Get_RfcEnable();
    WebConfigLog("paramCount = %d\n",paramCount);

    for(i=0; i<paramCount; i++)
    {
        if(parameterNames[i][strlen(parameterNames[i])-1] == '.')
        {
            isWildcard = 1;
        }
        else
        {
            isWildcard = 0;
        }
        for(j=0; j<objSize; j++)
        {
            if(strstr(parameterNames[i],objects[j]) != NULL)
            {
                matchFound = 1;
                switch(j)
                {
                    case 0:
                    {
                        if(isWildcard == 0)
                        {
                            paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                            memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                            if((strcmp(parameterNames[i], WEBCONFIG_PARAM_RFC_ENABLE) == 0))
                            {
                                paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_RFC_ENABLE, MAX_PARAMETERNAME_LEN);
                                WebcfgDebug("paramVal[%d]->parameterName: %s\n",k,paramVal[k]->parameterName);
                                WebcfgDebug("RfcEnable is %d\n",RFC_ENABLE);
                                if(RFC_ENABLE == true)
                                {
                                    paramVal[k]->parameterValue = strndup("true",MAX_PARAMETERVALUE_LEN);
                                }
                                else
                                {
                                    paramVal[k]->parameterValue = strndup("false",MAX_PARAMETERVALUE_LEN);
                                }
                                paramVal[k]->type = ccsp_boolean;
                                k++;
                            }
			    else if((strcmp(parameterNames[i], WEBCONFIG_PARAM_FORCE_SYNC) == 0) && (RFC_ENABLE == true))
                            {
				WebConfigLog("Force Sync GET is not supported\n");
                                paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_FORCE_SYNC, MAX_PARAMETERNAME_LEN);
				paramVal[k]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
                                paramVal[k]->type = ccsp_string;
                                k++;
				WebConfigLog("Webpa force sync done\n");
                            }
			    else if((strcmp(parameterNames[i], WEBCONFIG_PARAM_URL) == 0) && (RFC_ENABLE == true))
                            {
				char valuestr[256] = {0};
				WebConfigLog("B4 Get_Webconfig_URL\n");
				Get_Webconfig_URL(valuestr);
				if( (valuestr != NULL) && strlen(valuestr) >0 )
				{
					WebConfigLog("Webpa get : URL fetched %s\n", valuestr);
		                        paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_URL, MAX_PARAMETERNAME_LEN);
					paramVal[k]->parameterValue = strndup(valuestr,MAX_PARAMETERVALUE_LEN);
		                        paramVal[k]->type = ccsp_string;
		                        k++;
					WebConfigLog("Webpa get : URL done\n");
				}
				else
				{
					WebConfigLog("Webpa get : URL not found\n");
					WAL_FREE(paramVal[k]);
					matchFound = 0;
				}
                            }
                            else
                            {
                                WAL_FREE(paramVal[k]);
                                matchFound = 0;
                            }
                        }
                        else
                        {
                            if(RFC_ENABLE)
                            {
				WebConfigLog("Updating localCount %d in wildcard GET case\n", localCount);
                                localCount = localCount+3;
                            }
				WebConfigLog("Updated localCount %d\n", localCount);
                            paramVal = (parameterValStruct_t **) realloc(paramVal, sizeof(parameterValStruct_t *)*localCount);
                            paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                            memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                            paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_RFC_ENABLE, MAX_PARAMETERNAME_LEN);
                            WebcfgDebug("paramVal[%d]->parameterName: %s\n",k,paramVal[k]->parameterName);
                            WebcfgDebug("RfcEnable is %d\n",RFC_ENABLE);
                            if(RFC_ENABLE == true)
                            {
                                paramVal[k]->parameterValue = strndup("true",MAX_PARAMETERVALUE_LEN);
                            }
                            else
                            {
                                paramVal[k]->parameterValue = strndup("false",MAX_PARAMETERVALUE_LEN);
                            }
                            paramVal[k]->type = ccsp_boolean;
                            k++;
                            if(RFC_ENABLE)
                            {
                                WebConfigLog("Webpa wildcard get for force sync\n");
				paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                                memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                                paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_FORCE_SYNC, MAX_PARAMETERNAME_LEN);
				paramVal[k]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
                                paramVal[k]->type = ccsp_string;
                                k++;
				WebConfigLog("After Webpa wildcard get for force sync\n");

				WebConfigLog("Webpa wildcard get for URL\n");
				paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                                memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                                paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_URL, MAX_PARAMETERNAME_LEN);
				char webcfg_url[256] = {0};
				WebConfigLog("Wildcard get : B4 Get_Webconfig_URL\n");
				Get_Webconfig_URL(webcfg_url);
				if( (webcfg_url !=NULL) && strlen(webcfg_url)>0 )
				{
					WebConfigLog("webcfg_url fetched %s\n", webcfg_url);
					paramVal[k]->parameterValue = strndup(webcfg_url,MAX_PARAMETERVALUE_LEN);
					WebConfigLog("Wildcard get : paramVal[k]->parameterValue:%s\n", paramVal[k]->parameterValue);
				}
				else
				{
					paramVal[k]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
				}
                                paramVal[k]->type = ccsp_string;
                                k++;
				WebConfigLog("After Webpa wildcard get for URL\n");
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
        if(matchFound == 0)
        {
            if(!RFC_ENABLE)
            {
                WebConfigLog("RFC disabled. Hence not proceeding with GET\n");
            }
            else
            {
                WebConfigLog("%s is invalid parameter\n",parameterNames[i]);
            }
            *val = NULL;
            *val_size = 0;
            for(k=k-1;k>=0;k--)
            {
                WAL_FREE(paramVal[k]->parameterName);
                WAL_FREE(paramVal[k]->parameterValue);
                WAL_FREE(paramVal[k]);
            }
            WAL_FREE(paramVal);
            return CCSP_CR_ERR_UNSUPPORTED_NAMESPACE;
        }
    }
    *val = paramVal;
    for(i=0; i<k; i++)
    {
        WebConfigLog("Final-> %s %s %d\n",(*val)[i]->parameterName, (*val)[i]->parameterValue, (*val)[i]->type);
    }
    *val_size = k;
    WebConfigLog("Final count is %d\n",*val_size);
    WebConfigLog("*********** %s ***************\n",__FUNCTION__);
    return CCSP_SUCCESS;
}

int setWebConfigParameterValues(parameterValStruct_t *val, int paramCount, char **faultParam, char *transactionId )
{
	int i=0;
	char *subStr = NULL;
	BOOL RFC_ENABLE;
	int session_status = 0;
	int ret = 0;
	WebConfigLog("*********** %s ***************\n",__FUNCTION__);

	char *webConfigObject = "Device.X_RDK_WebConfig.";
	RFC_ENABLE = Get_RfcEnable();

	WebConfigLog("paramCount = %d\n",paramCount);
	for(i=0; i<paramCount; i++)
	{
		if(strstr(val[i].parameterName, webConfigObject) != NULL)
		{
			if(strcmp(val[i].parameterName, WEBCONFIG_PARAM_RFC_ENABLE) == 0)
			{
				if((val[i].parameterValue != NULL) && (strcmp(val[i].parameterValue, "true") == 0))
				{
					setRfcEnable(true);
				}
				else
				{
					setRfcEnable(false);
				}
			}
			else if((strcmp(val[i].parameterName, WEBCONFIG_PARAM_FORCE_SYNC) == 0) && (RFC_ENABLE == true))
			{
				WebConfigLog("Processing Force Sync param\n");
				if((val[i].parameterValue !=NULL) && (strlen(val[i].parameterValue)>0))
				{
					WebConfigLog("setWebConfigParameterValues setForceSync\n");
					ret = setForceSync(val[i].parameterValue, transactionId, &session_status);
					WebConfigLog("After setForceSync ret %d\n", ret);
				}
				else //pass empty transaction id when Force sync is with empty doc
				{
					WebConfigLog("setWebConfigParameterValues empty setForceSync\n");
					ret = setForceSync(val[i].parameterValue, "", 0);
				}
				if(session_status)
				{
					return CCSP_CR_ERR_SESSION_IN_PROGRESS;
				}
				if(!ret)
				{
					WebConfigLog("setForceSync failed\n");
					return CCSP_FAILURE;
				}
			}
			else if(!RFC_ENABLE)
			{
				WebConfigLog("RFC disabled. Hence not proceeding with SET\n");
				return CCSP_ERR_INVALID_PARAMETER_VALUE;
			}
		}
		else
		{
			WebConfigLog("%s is not writable\n",val[i].parameterName);
			*faultParam = strdup(val[i].parameterName);
			return CCSP_ERR_NOT_WRITABLE;
		}
	}
	WebConfigLog("*********** %s ***************\n",__FUNCTION__);
	return CCSP_SUCCESS;
}
