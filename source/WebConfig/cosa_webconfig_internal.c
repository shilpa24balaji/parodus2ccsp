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

#define WEBCONFIG_PARAM_RFC_ENABLE          "Device.X_RDK_WebConfig.RfcEnable"
#define WEBCONFIG_PARAM_CONFIGFILE_ENTRIES  "Device.X_RDK_WebConfig.ConfigFileNumberOfEntries"
#define WEBCONFIG_PARAM_PERIODIC_INTERVAL   "Device.X_RDK_WebConfig.PeriodicSyncCheckInterval"
#define WEBCONFIG_TABLE_CONFIGFILE          "Device.X_RDK_WebConfig.ConfigFile."
#define CONFIGFILE_PARAM_URL                "URL"
#define CONFIGFILE_PARAM_VERSION            "Version"
#define CONFIGFILE_PARAM_FORCE_SYNC         "ForceSyncCheck"
#define CONFIGFILE_PARAM_SYNC_CHECK_OK      "SyncCheckOK"
#define CONFIGFILE_PARAM_PREV_SYNC_TIME     "PreviousSyncDateTime"

extern PCOSA_BACKEND_MANAGER_OBJECT g_pCosaBEManager;

int getConfigNumberOfEntries()
{
	PCOSA_DATAMODEL_WEBCONFIG            pMyObject           = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	WalInfo("----------- %s --------- Enter -----\n",__FUNCTION__);
	int count = AnscSListQueryDepth( &pMyObject->ConfigFileList );
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return count;
}

int getInstanceNumberAtIndex(int index)
{
    PCOSA_DATAMODEL_WEBCONFIG            pMyObject           = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContextEntry = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    int count , i;
    WalInfo("----------- %s --------- Enter -----\n",__FUNCTION__);
    WalInfo("index: %d\n",index);
    count = getConfigNumberOfEntries();
    WalInfo("count: %d\n",count);
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->ConfigFileList);
    for(i = 0; i<count; i++)
    {
        pCosaContextEntry = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
        pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);
        if(index == i)
        {
            WalInfo("InstanceNumber at %d is: %d\n",i, pCosaContextEntry->InstanceNumber);
            return pCosaContextEntry->InstanceNumber;
        }
    }
    WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
    return 0;
}

BOOL getConfigURL(int index,char **configURL)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = NULL;
	//int i, count, indexFound = 0;
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	pConfigFileEntry = CosaSListGetEntryByInsNum(&pMyObject->ConfigFileList, index);
	if(NULL != pConfigFileEntry)
	{
	    WalInfo("pConfigFileEntry->URL: %s\n",pConfigFileEntry->URL);
	    if((pConfigFileEntry->URL)[0] != '\0')
		{
		    *configURL = strdup(pConfigFileEntry->URL);
	    }
	    else
	    {
	        *configURL = strdup("");
        }
        return TRUE;
	}
	/*count = getConfigNumberOfEntries();
	WalInfo("count = %d\n",count);
	WalInfo("index: %d\n",index);
	for(i=0;i<count;i++)
	{
	    WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(i));
		if(getInstanceNumberAtIndex(i) == index)
		{
			WalInfo("pConfigFileEntry[%d].URL: %s\n",i,pConfigFileEntry[i].URL);
			if((pConfigFileEntry[i].URL)[0] != '\0')
			{
			    *configURL = strdup(pConfigFileEntry[i].URL);
		    }
		    else
		    {
		        *configURL = strdup("");
	        }
			indexFound = 1;
			break;
		}
	}
	if(indexFound == 0)
	{
		WalError("Table with %d index is not available\n", index);
		return FALSE;
	}*/
	WalError("Table with %d index is not available\n", index);
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return FALSE;
}

int setConfigURL(int index, char *configURL)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = NULL;
	//int i, count, indexFound = 0;
	//char ParamName[MAX_BUFF_SIZE] = { 0 };
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	pConfigFileEntry = CosaSListGetEntryByInsNum(&pMyObject->ConfigFileList, index);
	if(NULL != pConfigFileEntry)
	{
	    AnscCopyString( pConfigFileEntry->URL, configURL );
	    CosaDmlSetConfigFileEntry(pConfigFileEntry);
	}
	/*count = getConfigNumberOfEntries();
	WalInfo("count : %d\n",count);
	for(i=0;i<count;i++)
	{
	    WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(i));
		if(getInstanceNumberAtIndex(i) == index)
		{
			AnscCopyString(pConfigFileEntry[i].URL,configURL);
			snprintf(ParamName,MAX_BUFF_SIZE, "configfile_%d_Url", index);
			CosaDmlStoreValueIntoDb(ParamName, configURL);
			indexFound = 1;
			break;
		}
	}
	if(indexFound == 0)
	{
		WalError("Table with %d index is not available\n", index);
		return 1;
	}*/
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return 0;
}

BOOL getPreviousSyncDateTime(int index,char **PreviousSyncDateTime)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = pMyObject->pConfigFileContainer->pConfigFileTable;
	int i, count, indexFound = 0;
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	count = getConfigNumberOfEntries();
	WalInfo("count : %d\n",count);
	for(i=0;i<count;i++)
	{
	    WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(i));
		if(getInstanceNumberAtIndex(i) == index)
		{
			*PreviousSyncDateTime = strdup(pConfigFileEntry[i].PreviousSyncDateTime);
			indexFound = 1;
			break;
		}
	}
	if(indexFound == 0)
	{
		WalError("Table with %d index is not available\n", index);
		return FALSE;
	}
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return TRUE;
}

int setPreviousSyncDateTime(int index)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = pMyObject->pConfigFileContainer->pConfigFileTable;
	int i, count, indexFound = 0;
	char ParamName[MAX_BUFF_SIZE] = { 0 };
	char current_time[MAX_BUFF_SIZE] = { 0 };
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	count = getConfigNumberOfEntries();
	WalInfo("count : %d\n",count);
	for(i=0;i<count;i++)
	{
	    WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(i));
		if(getInstanceNumberAtIndex(i) == index)
		{
			time_t curr_time = time(NULL);
			struct tm *tm = localtime(&curr_time);
			strftime(current_time, MAX_BUFF_SIZE, "%c", tm);
			AnscCopyString(pConfigFileEntry[i].PreviousSyncDateTime,current_time);
			snprintf(ParamName,MAX_BUFF_SIZE, "configfile_%d_SyncDateTime", index);
			CosaDmlStoreValueIntoDb(ParamName, current_time);
			indexFound = 1;
			break;
		}
	}
	if(indexFound == 0)
	{
		WalError("Table with %d index is not available\n", index);
		return 1;
	}
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return 0;
}

BOOL getConfigVersion(int index, char **version)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = pMyObject->pConfigFileContainer->pConfigFileTable;
	int i, count, indexFound = 0;
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	count = getConfigNumberOfEntries();
	WalInfo("count : %d\n",count);

	for(i=0;i<count;i++)
	{
	    WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(i));
		if(getInstanceNumberAtIndex(i) == index)
		{
			*version = strdup(pConfigFileEntry[i].Version);
			indexFound = 1;
			break;
		}
	}
	if(indexFound == 0)
	{
		WalError("Table with %d index is not available\n", index);
		return FALSE;
	}
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return TRUE;
}

int setConfigVersion(int index, char *version)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = pMyObject->pConfigFileContainer->pConfigFileTable;
	int i, count, indexFound = 0;
	char ParamName[MAX_BUFF_SIZE] = { 0 };
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	count = getConfigNumberOfEntries();
	WalInfo("count : %d\n",count);
	for(i=0;i<count;i++)
	{
	    WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(i));
		WalInfo("Inside setConfigVersion for\n");
		if(getInstanceNumberAtIndex(i) == index)
		{
			AnscCopyString(pConfigFileEntry[i].Version,version);
			snprintf(ParamName,MAX_BUFF_SIZE, "configfile_%d_Version", index);
			CosaDmlStoreValueIntoDb(ParamName, version);
			indexFound = 1;
			break;
		}
	}
	if(indexFound == 0)
	{
		WalError("Table with %d index is not available\n", index);
		return 1;
	}
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return 0;
}

BOOL getSyncCheckOK(int index)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = pMyObject->pConfigFileContainer->pConfigFileTable;
	int i, count, indexFound = 0;
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	count = getConfigNumberOfEntries();
	WalInfo("count : %d\n",count);
	for(i=0;i<count;i++)
	{
		if(getInstanceNumberAtIndex(i) == index)
		{
			return pConfigFileEntry[i].SyncCheckOK;
		}
	}
	WalError("Table with %d index is not available\n", index);
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return FALSE;
}

int setSyncCheckOK(int index, BOOL status)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = pMyObject->pConfigFileContainer->pConfigFileTable;
	int i, count, indexFound = 0;
	char ParamName[MAX_BUFF_SIZE] = { 0 };
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	count = getConfigNumberOfEntries();
	WalInfo("count : %d\n",count);
	for(i=0;i<count;i++)
	{
		WalInfo("Inside setSyncCheckOK for\n");
		if(getInstanceNumberAtIndex(i) == index)
		{
			pConfigFileEntry[i].SyncCheckOK = status;
			snprintf(ParamName,MAX_BUFF_SIZE, "configfile_%d_SyncCheckOk", index);
			if(status == true)
			{
				CosaDmlStoreValueIntoDb(ParamName, "true");
			}
			else
			{
				CosaDmlStoreValueIntoDb(ParamName, "false");
			}
			indexFound = 1;
			break;
		}
	}
	if(indexFound == 0)
	{
		WalError("Table with %d index is not available\n", index);
		return 1;
	}
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return 0;
}

BOOL getForceSyncCheck(int index)
{
	PCOSA_DATAMODEL_WEBCONFIG  pMyObject = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;
	PCOSA_DML_WEBCONFIG_CONFIGFILE_ENTRY pConfigFileEntry    = pMyObject->pConfigFileContainer->pConfigFileTable;
	int i, count, indexFound = 0;
	WalInfo("-------- %s ----- Enter ------\n",__FUNCTION__);
	count = getConfigNumberOfEntries();
	WalInfo("count : %d\n",count);
	for(i=0;i<count;i++)
	{
	    WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(i));
		if(getInstanceNumberAtIndex(i) == index)
		{
			return pConfigFileEntry[i].ForceSyncCheck;
		}
	}
	WalError("Table with %d index is not available\n", index);
	WalInfo("-------- %s ----- Exit ------\n",__FUNCTION__);
	return FALSE;
}

void updateParamValStructWIthConfigFileDataAtIndex(parameterValStruct_t **paramVal, int index, int valIndex, int *finalIndex)
{
	int ret = 0;
	char *valueStr = NULL;
	BOOL bValue = false;
	WalInfo("--------- %s ------ ENter -----\n",__FUNCTION__);
	paramVal[valIndex] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
	memset(paramVal[valIndex], 0, sizeof(parameterValStruct_t));
	paramVal[valIndex]->parameterName = (char *)malloc(sizeof(char)*MAX_PARAMETERNAME_LEN);
	snprintf(paramVal[valIndex]->parameterName, MAX_PARAMETERNAME_LEN, "%s%d.%s",WEBCONFIG_TABLE_CONFIGFILE, index, CONFIGFILE_PARAM_URL);
	ret = getConfigURL(index, &valueStr); 
	if(ret)
	{
		WalInfo("valueStr: %s\n",valueStr);
		paramVal[valIndex]->parameterValue = strndup(valueStr,MAX_PARAMETERVALUE_LEN);
		WAL_FREE(valueStr);
	}
	else
	{
		paramVal[valIndex]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
	}
	paramVal[valIndex]->type = ccsp_string;
	valIndex++;
	paramVal[valIndex] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
	memset(paramVal[valIndex], 0, sizeof(parameterValStruct_t));
	paramVal[valIndex]->parameterName = (char *)malloc(sizeof(char)*MAX_PARAMETERNAME_LEN);
	snprintf(paramVal[valIndex]->parameterName, MAX_PARAMETERNAME_LEN, "%s%d.%s",WEBCONFIG_TABLE_CONFIGFILE, index, CONFIGFILE_PARAM_VERSION);
	ret = getConfigVersion(index, &valueStr); 
	if(ret)
	{
		WalInfo("valueStr: %s\n",valueStr);
		paramVal[valIndex]->parameterValue = strndup(valueStr,MAX_PARAMETERVALUE_LEN);
		WAL_FREE(valueStr);
	}
	else
	{
		paramVal[valIndex]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
	}
	paramVal[valIndex]->type = ccsp_string;
	valIndex++;
	paramVal[valIndex] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
	memset(paramVal[valIndex], 0, sizeof(parameterValStruct_t));
	paramVal[valIndex]->parameterName = (char *)malloc(sizeof(char)*MAX_PARAMETERNAME_LEN);
	snprintf(paramVal[valIndex]->parameterName, MAX_PARAMETERNAME_LEN, "%s%d.%s",WEBCONFIG_TABLE_CONFIGFILE, index, CONFIGFILE_PARAM_FORCE_SYNC);
	bValue = getForceSyncCheck(index);
    WalInfo("ForceSyncCheck is %d\n",bValue);
    if(bValue == true)
    {
        paramVal[valIndex]->parameterValue = strndup("true",MAX_PARAMETERVALUE_LEN);
    }
    else
    {
        paramVal[valIndex]->parameterValue = strndup("false",MAX_PARAMETERVALUE_LEN);
    }
	paramVal[valIndex]->type = ccsp_boolean;
	valIndex++;
	paramVal[valIndex] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
	memset(paramVal[valIndex], 0, sizeof(parameterValStruct_t));
	paramVal[valIndex]->parameterName = (char *)malloc(sizeof(char)*MAX_PARAMETERNAME_LEN);
	snprintf(paramVal[valIndex]->parameterName, MAX_PARAMETERNAME_LEN, "%s%d.%s",WEBCONFIG_TABLE_CONFIGFILE, index, CONFIGFILE_PARAM_SYNC_CHECK_OK);
	bValue = getSyncCheckOK(index);
    WalInfo("SyncCheckOK is %d\n",bValue);
    if(bValue == true)
    {
        paramVal[valIndex]->parameterValue = strndup("true",MAX_PARAMETERVALUE_LEN);
    }
    else
    {
        paramVal[valIndex]->parameterValue = strndup("false",MAX_PARAMETERVALUE_LEN);
    }
	paramVal[valIndex]->type = ccsp_boolean;
	valIndex++;
	paramVal[valIndex] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
	memset(paramVal[valIndex], 0, sizeof(parameterValStruct_t));
	paramVal[valIndex]->parameterName = (char *)malloc(sizeof(char)*MAX_PARAMETERNAME_LEN);
	snprintf(paramVal[valIndex]->parameterName, MAX_PARAMETERNAME_LEN, "%s%d.%s",WEBCONFIG_TABLE_CONFIGFILE, index, CONFIGFILE_PARAM_PREV_SYNC_TIME);
	ret = getPreviousSyncDateTime(index, &valueStr); 
	if(ret)
	{
		WalInfo("valueStr: %s\n",valueStr);
		paramVal[valIndex]->parameterValue = strndup(valueStr,MAX_PARAMETERVALUE_LEN);
		WAL_FREE(valueStr);
	}
	else
	{
		paramVal[valIndex]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
	}
	paramVal[valIndex]->type = ccsp_string;
	valIndex++;
	*finalIndex = valIndex;
	WalInfo("*finalIndex: %d\n",*finalIndex);
	WalInfo("--------- %s ------ EXIT -----\n",__FUNCTION__);
}

int getWebConfigParameterValues(char **parameterNames, int paramCount, int *val_size, parameterValStruct_t ***val)
{
    char *objects[] ={"Device.X_RDK_WebConfig.ConfigFile.","Device.X_RDK_WebConfig."};
    int objSize = sizeof(objects)/sizeof(objects[0]);
    parameterValStruct_t **paramVal = NULL;
    paramVal = (parameterValStruct_t **) malloc(sizeof(parameterValStruct_t *)*paramCount);
    int i=0, j=0, k=0, isWildcard = 0, matchFound = 0;
    int localCount = paramCount;
    WalInfo("*********** %s ***************\n",__FUNCTION__);

    PCOSA_DATAMODEL_WEBCONFIG   pWebConfig = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;

    WalInfo("paramCount = %d\n",paramCount);
    int count = getConfigNumberOfEntries();
    WalInfo("count: %d\n",count);
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
                            char* instNumStart = NULL, *valueStr = NULL;
                            char restDmlString[128] = {'\0'};
                            int index = 0, ret = 0;
                            WalInfo("parameterNames[%d]: %s\n",i,parameterNames[i]);
                            instNumStart = parameterNames[i]+strlen(WEBCONFIG_TABLE_CONFIGFILE);
                            WalInfo("instNumStart: %s\n",instNumStart);
                            sscanf(instNumStart, "%d.%s", &index, restDmlString);
                            WalInfo("index: %d restDmlString: %s\n",index,restDmlString);
                            
                            paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                            memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                            if(strcmp(restDmlString, CONFIGFILE_PARAM_URL) == 0)
                            {
                                paramVal[k]->parameterName = strndup(parameterNames[i], MAX_PARAMETERNAME_LEN);
								ret = getConfigURL(index, &valueStr);
								if(ret)
								{
									WalInfo("valueStr: %s\n",valueStr);
									paramVal[k]->parameterValue = strndup(valueStr,MAX_PARAMETERVALUE_LEN);
									WAL_FREE(valueStr);
								}
								else
								{
									paramVal[k]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
								}
								paramVal[k]->type = ccsp_string;
                                k++;
                            }
                            else if(strcmp(restDmlString, CONFIGFILE_PARAM_VERSION) == 0)
                            {
                                paramVal[k]->parameterName = strndup(parameterNames[i], MAX_PARAMETERNAME_LEN);
                                ret = getConfigVersion(index, &valueStr);
								if(ret)
								{
									WalInfo("valueStr: %s\n",valueStr);
									paramVal[k]->parameterValue = strndup(valueStr,MAX_PARAMETERVALUE_LEN);
									WAL_FREE(valueStr);
								}
								else
								{
									paramVal[k]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
								}
								paramVal[k]->type = ccsp_string;
                                k++;
                            }
                            else if(strcmp(restDmlString, CONFIGFILE_PARAM_FORCE_SYNC) == 0)
                            {
                                paramVal[k]->parameterName = strndup(parameterNames[i], MAX_PARAMETERNAME_LEN);
								BOOL bValue = getForceSyncCheck(index);
                                WalInfo("ForceSyncCheck is %d\n",bValue);
                                if(bValue == true)
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
                            else if(strcmp(restDmlString, CONFIGFILE_PARAM_SYNC_CHECK_OK) == 0)
                            {
                                paramVal[k]->parameterName = strndup(parameterNames[i], MAX_PARAMETERNAME_LEN);
                                BOOL bValue = getSyncCheckOK(index);
                                WalInfo("SyncCheckOK is %d\n",bValue);
                                if(bValue == true)
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
                            else if(strcmp(restDmlString, CONFIGFILE_PARAM_PREV_SYNC_TIME) == 0)
                            {
                                paramVal[k]->parameterName = strndup(parameterNames[i], MAX_PARAMETERNAME_LEN);
                                ret = getPreviousSyncDateTime(index, &valueStr);
								if(ret)
								{
									WalInfo("valueStr: %s\n",valueStr);
									paramVal[k]->parameterValue = strndup(valueStr,MAX_PARAMETERVALUE_LEN);
									WAL_FREE(valueStr);
								}
								else
								{
									paramVal[k]->parameterValue = strndup("",MAX_PARAMETERVALUE_LEN);
								}
								paramVal[k]->type = ccsp_string;
                                k++;
                            }
                            else
                            {
                                WAL_FREE(paramVal[k]);
                                matchFound = 0;
                            }
                        }
                        else
                        {
                            int index = 0;
                            if(strcmp(parameterNames[i], WEBCONFIG_TABLE_CONFIGFILE) == 0)
                            {
                                if(count > 0)
                                {
                                    localCount = localCount+(count*5)-1;
                                    paramVal = (parameterValStruct_t **) realloc(paramVal, sizeof(parameterValStruct_t *)*localCount);
                                    int n = 0, index = 0;
                                    for(n = 0; n<count; n++)
                                    {
                                        WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(n));
                                        index = getInstanceNumberAtIndex(n);
                                        WalInfo("B4 updateParamValStructWIthConfigFileDataAtIndex\n");
								        updateParamValStructWIthConfigFileDataAtIndex(paramVal, index, k, &k);
								        WalInfo("k = %d\n",k);
                                    }                    
                                }
                                else
                                {
                                    paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                                    memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                                    paramVal[k]->parameterName = strndup(WEBCONFIG_TABLE_CONFIGFILE, MAX_PARAMETERNAME_LEN);
                                    paramVal[k]->parameterValue = strndup("EMPTY",MAX_PARAMETERVALUE_LEN);
                                    k++;
                                }
                            }
                            else
                            {
                                char* instNumStart = NULL;
                                WalInfo("parameterNames[%d]: %s\n",i, parameterNames[i]);
                                instNumStart = parameterNames[i]+strlen(WEBCONFIG_TABLE_CONFIGFILE);
                                WalInfo("instNumStart: %s\n",instNumStart);
                                sscanf(instNumStart, "%d.", &index);
								WalInfo("index: %d\n",index);
                                localCount = localCount+4;
                                paramVal = (parameterValStruct_t **) realloc(paramVal, sizeof(parameterValStruct_t *)*localCount);
								WalInfo("B4 updateParamValStructWIthConfigFileDataAtIndex\n");
								updateParamValStructWIthConfigFileDataAtIndex(paramVal, index, k, &k);
								WalInfo("k = %d\n",k);
                            }
                        }
                        break;
                    }
					case 1:
                    {
                        if(isWildcard == 0)
                        {
                            paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                            memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                            if((strcmp(parameterNames[i], WEBCONFIG_PARAM_RFC_ENABLE) == 0))
                            {
                                paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_RFC_ENABLE, MAX_PARAMETERNAME_LEN);
                                WalInfo("paramVal[%d]->parameterName: %s\n",k,paramVal[k]->parameterName);
                                WalInfo("pWebConfig->RfcEnable is %d\n",pWebConfig->RfcEnable);
                                if(pWebConfig->RfcEnable == true)
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
                            else if(strcmp(parameterNames[i], WEBCONFIG_PARAM_CONFIGFILE_ENTRIES) == 0)
                            {
                                paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_CONFIGFILE_ENTRIES, MAX_PARAMETERNAME_LEN);
                                int count = getConfigNumberOfEntries();
                                WalInfo("count is %d\n",count);
                                paramVal[k]->parameterValue = (char *)malloc(sizeof(char)*MAX_PARAMETERVALUE_LEN);
                                snprintf(paramVal[k]->parameterValue,MAX_PARAMETERVALUE_LEN,"%d",count);
                                paramVal[k]->type = ccsp_unsignedInt;
                                k++;
                            }
                            else if(strcmp(parameterNames[i], WEBCONFIG_PARAM_PERIODIC_INTERVAL) == 0)
                            {
                                paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_PERIODIC_INTERVAL, MAX_PARAMETERNAME_LEN);
                                WalInfo("pWebConfig->PeriodicSyncCheckInterval is %d\n",pWebConfig->PeriodicSyncCheckInterval);
                                paramVal[k]->parameterValue = (char *)malloc(sizeof(char)*MAX_PARAMETERVALUE_LEN);
                                snprintf(paramVal[k]->parameterValue,MAX_PARAMETERVALUE_LEN,"%d",pWebConfig->PeriodicSyncCheckInterval);
                                paramVal[k]->type = ccsp_int;
                                k++;
                            }
                            else
                            {
                                WAL_FREE(paramVal[k]);
                                matchFound = 0;
                            }
                        }
                        else
                        {
                            if(count > 0)
                            {
                                localCount = localCount+2+(count*5);
                            }
                            else
                            {
                                localCount= localCount+2;
                            }
                            paramVal = (parameterValStruct_t **) realloc(paramVal, sizeof(parameterValStruct_t *)*localCount);
                            paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                            memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                            paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_RFC_ENABLE, MAX_PARAMETERNAME_LEN);
                            WalInfo("paramVal[%d]->parameterName: %s\n",k,paramVal[k]->parameterName);
                            WalInfo("pWebConfig->RfcEnable is %d\n",pWebConfig->RfcEnable);
                            if(pWebConfig->RfcEnable == true)
                            {
                                paramVal[k]->parameterValue = strndup("true",MAX_PARAMETERVALUE_LEN);
                            }
                            else
                            {
                                paramVal[k]->parameterValue = strndup("false",MAX_PARAMETERVALUE_LEN);
                            }
                            paramVal[k]->type = ccsp_boolean;
                            k++;
                            paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                            memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                            paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_CONFIGFILE_ENTRIES, MAX_PARAMETERNAME_LEN);
                            WalInfo("paramVal[%d]->parameterName: %s\n",k,paramVal[k]->parameterName);
                            paramVal[k]->parameterValue = (char *)malloc(sizeof(char)*MAX_PARAMETERVALUE_LEN);
                            WalInfo("count is %d\n",count);
                            snprintf(paramVal[k]->parameterValue,MAX_PARAMETERVALUE_LEN,"%d",count);
                            paramVal[k]->type = ccsp_unsignedInt;
                            k++;
                            paramVal[k] = (parameterValStruct_t *) malloc(sizeof(parameterValStruct_t));
                            memset(paramVal[k], 0, sizeof(parameterValStruct_t));
                            paramVal[k]->parameterName = strndup(WEBCONFIG_PARAM_PERIODIC_INTERVAL, MAX_PARAMETERNAME_LEN);
                            WalInfo("pWebConfig->PeriodicSyncCheckInterval is %d\n",pWebConfig->PeriodicSyncCheckInterval);
                            paramVal[k]->parameterValue = (char *)malloc(sizeof(char)*MAX_PARAMETERVALUE_LEN);
                            snprintf(paramVal[k]->parameterValue,MAX_PARAMETERVALUE_LEN,"%d",pWebConfig->PeriodicSyncCheckInterval);
                            paramVal[k]->type = ccsp_int;
                            k++;
                            int n = 0, index = 0;
                            for(n = 0; n<count; n++)
                            {
                                WalInfo("InstNum: %d\n",getInstanceNumberAtIndex(n));
                                index = getInstanceNumberAtIndex(n);
                                WalInfo("B4 updateParamValStructWIthConfigFileDataAtIndex\n");
						        updateParamValStructWIthConfigFileDataAtIndex(paramVal, index, k, &k);
						        WalInfo("k = %d\n",k);
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
            WalError("%s is invalid parameter\n",parameterNames[i]);
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
        WalInfo("Final-> %s %s %d\n",(*val)[i]->parameterName, (*val)[i]->parameterValue, (*val)[i]->type);
    }
    *val_size = k;
    WalInfo("Final count is %d\n",*val_size);
    WalInfo("*********** %s ***************\n",__FUNCTION__);
    return CCSP_SUCCESS;
}

int setWebConfigParameterValues(parameterValStruct_t *val, int paramCount, char **faultParam )
{
    int i=0;
    char *subStr = NULL;
    WalInfo("*********** %s ***************\n",__FUNCTION__);

    char *webConfigObject = "Device.X_RDK_WebConfig.";
    PCOSA_DATAMODEL_WEBCONFIG   pWebConfig = (PCOSA_DATAMODEL_WEBCONFIG)g_pCosaBEManager->hWebConfig;

    WalInfo("paramCount = %d\n",paramCount);
    for(i=0; i<paramCount; i++)
    {
        if(strstr(val[i].parameterName, webConfigObject) != NULL)
        {
            if(strcmp(val[i].parameterName, WEBCONFIG_PARAM_RFC_ENABLE) == 0)
            {
				CosaDmlStoreValueIntoDb( "WebConfigRfcEnabled", val[i].parameterValue );
				if(val[i].parameterValue != NULL && atoi(val[i].parameterValue) == 1)
				{
					pWebConfig->RfcEnable = true;
				}
				else
				{
					pWebConfig->RfcEnable = false;
				}
            }
            else if(strcmp(val[i].parameterName, WEBCONFIG_PARAM_PERIODIC_INTERVAL) == 0)
            {
                subStr = val[i].parameterName+strlen(webConfigObject);
				CosaDmlStoreValueIntoDb( subStr, val[i].parameterValue );
				if(val[i].parameterValue != NULL)
				{
                	pWebConfig->PeriodicSyncCheckInterval = atoi(val[i].parameterValue);
				}
				else
				{
					pWebConfig->PeriodicSyncCheckInterval = 0;
				}
            }
            else if(strstr(val[i].parameterName, WEBCONFIG_TABLE_CONFIGFILE) != NULL)
            {
                subStr = val[i].parameterName+strlen(WEBCONFIG_TABLE_CONFIGFILE);
                int index = 0, ret = 0;
                char dmlString[128] = {'\0'};
                sscanf(subStr, "%d.%s",&index, dmlString);
				WalInfo("index: %d dmlString:%s\n",index, dmlString);
                if(strcmp(dmlString, CONFIGFILE_PARAM_URL) == 0)
                {
                    if(isValidUrl(val[i].parameterValue) == TRUE)
                    {
                        ret = setConfigURL(index, val[i].parameterValue);
                        if(ret != 0)
                        {
                            WalError("SET failed\n");
                            return CCSP_FAILURE;
                        }
                    }
                    else
                    {
                        WalError("URL validation failed\n");
                        return CCSP_FAILURE;
                    }
                }
                else if(strcmp(dmlString, CONFIGFILE_PARAM_VERSION) == 0)
                {
                    ret = setConfigVersion(index, val[i].parameterValue);
                    if(ret != 0)
                    {
                        WalError("SET failed\n");
                        return CCSP_FAILURE;
                    }
                }
                else if(strcmp(dmlString, CONFIGFILE_PARAM_PREV_SYNC_TIME) == 0)
                {
                    ret = setPreviousSyncDateTime(index);
                    if(ret != 0)
                    {
                        WalError("SET failed\n");
                        return CCSP_FAILURE;
                    }
                }
                else if(strcmp(dmlString, CONFIGFILE_PARAM_SYNC_CHECK_OK) == 0)
                {
                    int syncCheck = 0;
                    sscanf(val[i].parameterValue, "%d",&syncCheck);
                    if(syncCheck == 1)
                    {
                        ret = setSyncCheckOK(index, true);
                    }
                    else
                    {
                        ret = setSyncCheckOK(index, false);
                    }
                    if(ret != 0)
                    {
                        WalError("SET failed\n");
                        return CCSP_FAILURE;
                    }
                }
            }
        }
        else
        {
            WalError("%s is not writable\n",val[i].parameterName);
            *faultParam = strdup(val[i].parameterName);
            return CCSP_ERR_NOT_WRITABLE;
        }
    }
    WalInfo("*********** %s ***************\n",__FUNCTION__);
    return CCSP_SUCCESS;
}
