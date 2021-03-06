/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>
#include <utils/threads.h>
#include <gui/Sensor.h>
#include <gui/SensorManager.h>
#include <gui/SensorEventQueue.h>
#include <vector>
#include <string>



char const* getSensorName(int type);

using namespace android;

struct SensorAttribute
{
    SensorAttribute(Sensor const* lp_new_sensor):
        m_lp_sensor(lp_new_sensor), m_IsEnable(false)
    {;}

    Sensor const* m_lp_sensor;
    bool m_IsEnable;
    bool m_IsRequestWakeUp;
};


typedef std::vector<SensorAttribute> SensorCollection;



class SensorThread : public Thread
{
public:
    SensorThread(sp<SensorEventQueue>& queue, int sample, int incalibrate)
            : mSensorEventQueue(queue), mSamples(sample), mIncalibrate(incalibrate), mCount(0) { }
    ~SensorThread() { }

private:
    virtual bool threadLoop();
    sp<SensorEventQueue> mSensorEventQueue;
    int mSamples;
    int mIncalibrate;
    int mCount;
};


bool SensorThread::threadLoop()
{
    ASensorEvent event[16];
    ssize_t n;

    if(mSamples == 0) {
        requestExit();
        return false;
    }

    mSensorEventQueue->waitForEvent();
    n = mSensorEventQueue->read(event, 16);

    if (mSamples != 0 && n > 0) {
        for (int i = 0; i < n; i++) {
            /* For Android KK, there is a META_DATA type to indicate flush
               operation */
            if (event[i].type == SENSOR_TYPE_META_DATA) {
                continue;
            }
            switch (event[i].type) {
                case SENSOR_TYPE_MAGNETIC_FIELD:
                case SENSOR_TYPE_ORIENTATION:
                    if (mIncalibrate == 1) {
                        if ((event[i].magnetic.status == 3) && (mCount > 30)) {
                            printf("calibrate successful type %d\n", event[i].type);
                            requestExit();
                            return false;
                        }
                        else {
                            mCount++;
                            printf("calibrating, accuracy=%d, type %d\n", event[i].magnetic.status,event[i].type);
                        }
                    }
                    else {
                        printf("value=<%9.4f,%9.4f,%9.4f>, time=%lld, accuracy=%d, sensor=%s\n",
                               event[i].data[0], event[i].data[1], event[i].data[2],
                               (long long int)event[i].timestamp, event[i].magnetic.status, getSensorName(event[i].type));
                    }
                    break;
                case SENSOR_TYPE_STEP_COUNTER:
                    printf("steps=%llu, time=%lld, sensor=%s\n",
                           (long long unsigned int)event[i].u64.step_counter,
                           (long long int)event[i].timestamp, getSensorName(event[i].type));
                    break;
                case SENSOR_TYPE_ROTATION_MATRIX:
#if 0
                    printf("value=<%9.4f,%9.4f,%9.4f,\n       %9.4f,%9.4f,%9.4f,\n       %9.4f,%9.4f,%9.4f>\n",
                    //printf("value=<%d,%d,%d,\n       %d,%d,%d,\n       %d,%d,%d>\n",
                           event[i].data[0], event[i].data[1], event[i].data[2],
                           event[i].data[3], event[i].data[4], event[i].data[5],
                           event[i].data[6], event[i].data[7], event[i].data[8]);
                    printf("time=%lld, sensor=%s\n",
                           (long long int)event[i].timestamp, getSensorName(event[i].type));
#else
					printf("%9.4f,%9.4f,%9.4f,%9.4f,%9.4f,%9.4f,%lld\n",
						event[i].data[0], event[i].data[1], event[i].data[2],
						event[i].data[3], event[i].data[4], event[i].data[5],
						(long long int)event[i].timestamp/1000000);

#endif
                    break;
                default:
                    printf("value=<%9.4f,%9.4f,%9.4f>, time=%lld, sensor=%s\n",
                           event[i].data[0], event[i].data[1], event[i].data[2],
                           (long long int)event[i].timestamp, getSensorName(event[i].type));
                    break;
            }
            mSamples--;
            if (mSamples == 0)
                break;
        }
    }

    return true;
}


char const* short_options = "a:d:s:b:vlw";

struct option long_options[] = {
        {"action" ,  required_argument, 0,  'a'},
        {"delay"  ,  required_argument, 0,  'd'},
        {"sample" ,  required_argument, 0,  's'},
        {"verbose",  no_argument      , 0,  'v'},
        {"list"   ,  no_argument      , 0,  'l'},
        {"batch" ,   required_argument, 0,  'b'},
        {"wake"   ,  no_argument      , 0,  'w'},
        {0        ,  0                , 0,   0 }
};

struct SensorNameTypeMap
{
    int Type;
    const char* lpName;
};

static struct SensorNameTypeMap g_SensorNameTypeMap[] = 
{
    { SENSOR_TYPE_ACCELEROMETER, "Acc" },
    { SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED, "Mag" },
    { SENSOR_TYPE_MAGNETIC_FIELD, "MagCal" },
    { SENSOR_TYPE_ORIENTATION, "Ori" },
    { SENSOR_TYPE_GYROSCOPE, "Gyr" },
    { SENSOR_TYPE_LIGHT, "Lux" },
    { SENSOR_TYPE_PRESSURE, "Bar" },
    { SENSOR_TYPE_TEMPERATURE, "Tmp" },
    { SENSOR_TYPE_PROXIMITY, "Prx" },
    { SENSOR_TYPE_GRAVITY, "Grv" },
    { SENSOR_TYPE_LINEAR_ACCELERATION, "Lac" },
    { SENSOR_TYPE_ROTATION_VECTOR, "Rot" },
    { SENSOR_TYPE_RELATIVE_HUMIDITY, "Hum" },
    { SENSOR_TYPE_AMBIENT_TEMPERATURE, "Tam" },
    { SENSOR_TYPE_ROTATION_MATRIX, "Rmx" },
    { SENSOR_TYPE_ACC_RAW, "Acc_raw" },
    { SENSOR_TYPE_GYRO_RAW, "Gyro_raw" },
    { SENSOR_TYPE_COMPS_RAW, "Comps_raw"},
};

static void DisplaySensorParameterName(void)
{
    int Loop_i = 0;
    const int Loop_End = sizeof(g_SensorNameTypeMap)/sizeof(g_SensorNameTypeMap[1]);

    printf("Sensor parameters may support:\n\n");

    for (Loop_i; Loop_i < Loop_End; ++Loop_i) {
        printf("Name: %s, ID = %d\n",
               g_SensorNameTypeMap[Loop_i].lpName,
               g_SensorNameTypeMap[Loop_i].Type);
    }

    printf("\n");

    return;
}

const char* getSensorName(int RequestType)
{
    static const char* DummyName = "";

    int Loop_i = 0;
    const int Loop_End = sizeof(g_SensorNameTypeMap)/sizeof(g_SensorNameTypeMap[1]);
    
    for (Loop_i; Loop_i < Loop_End; ++Loop_i) {
        if (RequestType == g_SensorNameTypeMap[Loop_i].Type) {
            return g_SensorNameTypeMap[Loop_i].lpName;
        }
    }

    return DummyName;
}

int getSensorType(char const* RequestName)
{
    int Loop_i = 0;
    const int Loop_End = sizeof(g_SensorNameTypeMap)/sizeof(g_SensorNameTypeMap[1]);
    
    for (Loop_i; Loop_i < Loop_End; ++Loop_i) {
        if (0 == strcasecmp(RequestName, g_SensorNameTypeMap[Loop_i].lpName)) {
            return g_SensorNameTypeMap[Loop_i].Type;
        }
    }

    if (isdigit(RequestName[0])) {
        return atoi(RequestName);
    }

    return -1;

}


int skipThisSensor(int type, int* types, int numTypes) {
    for (int i=0; i<numTypes; i++) {
        if (type == types[i]) {
            return 0;
        }
    }

    return numTypes != 0;
}

static void SensorList2SensorCollection(SensorCollection& AllSensor,
                                        Sensor const* const* lpList, int Count)
{
    int Loop_i;

    if (!lpList || 0 == Count) {
        return;
    }

    for (Loop_i = 0; Loop_i < Count; ++Loop_i) {
        AllSensor.push_back(SensorAttribute(lpList[Loop_i]));
    }

    return;
}

static void DumpExistSensor(SensorCollection& AllSensor)
{
    SensorCollection::iterator IterBegin = AllSensor.begin();
    SensorCollection::iterator IterEnd = AllSensor.end();

    if (AllSensor.empty()) {
        printf("Not found any exist sensors.\n");
    }

    IterEnd = AllSensor.end();
    for (; IterBegin != IterEnd; ++IterBegin) {
        printf("Name: %-32s| Vendor: %-28s | Handle: %10d | type: %5d | parameter name: %s\n",
               (*IterBegin).m_lp_sensor->getName().string(),
               (*IterBegin).m_lp_sensor->getVendor().string(),
               (*IterBegin).m_lp_sensor->getHandle(),
               (*IterBegin).m_lp_sensor->getType(),
               getSensorName((*IterBegin).m_lp_sensor->getType())
               );
    }

    return;
}


static bool IsNeedWakup(int Type)
{
    if (Type == SENSOR_TYPE_PROXIMITY
        || Type == SENSOR_TYPE_SIGNIFICANT_MOTION
        || Type == SENSOR_TYPE_TILT_DETECTOR
        || Type == SENSOR_TYPE_WAKE_GESTURE
        || Type == SENSOR_TYPE_GLANCE_GESTURE
        || Type == SENSOR_TYPE_PICK_UP_GESTURE) {
        return true;
    }

    return false;
}

static bool SelectRequestSensor(SensorCollection& AllSensor,
                                std::vector<std::string>& RequestSensor)
{
    unsigned int Loop_i = 0;
    int SensorType;
    int IsWakeup;
    bool RetValue;

    SensorCollection::iterator Begin, End;

    RetValue = false;
    for(; Loop_i < RequestSensor.size(); ++Loop_i) {
        /*
          get type by parameter name
          check wake up state by type

         */
        SensorType = getSensorType(RequestSensor[Loop_i].c_str());
        if (-1 == SensorType) {
            printf("Unknow sensor name: %s, Ignored.\n",
                   RequestSensor[Loop_i].c_str());
            continue;
        }

        IsWakeup = IsNeedWakup(SensorType);

        /*
          get special sensor by both type and wake up
         */
        Begin = AllSensor.begin();
        End = AllSensor.end();
        for (; Begin != End; ++Begin) {
            printf("gettype() = %d, iswakup = %d | Sensortype = %d, wakup = %d \n",
                   (*Begin).m_lp_sensor->getType(),
                   (*Begin).m_lp_sensor->isWakeUpSensor(),
                   SensorType, IsWakeup);

            if ((*Begin).m_lp_sensor->getType() == SensorType
                && (*Begin).m_lp_sensor->isWakeUpSensor() == IsWakeup) {
                (*Begin).m_IsEnable = true;
                (*Begin).m_IsRequestWakeUp = IsWakeup;
                RetValue = true;
            }
        }
    }

    printf("RetValue = %d\n", RetValue);
    return RetValue;
}


static void EnableSelectedSensor(SensorCollection& AllSensor,
                                 sp<SensorEventQueue> SensorDataQueue,
                                 int DelayTime, int BatchTime)
{
    SensorCollection::iterator Begin, End;

    int Delay = ns2us(ms2ns(DelayTime));
    int Batch = ns2us(ms2ns(BatchTime));
    int IsFiFo;

    End = AllSensor.end();
    for(Begin = AllSensor.begin(); Begin != End; ++Begin) {
		if ((*Begin).m_IsEnable) {
            IsFiFo = (*Begin).m_IsRequestWakeUp ? SENSORS_BATCH_WAKE_UPON_FIFO_FULL : 0;
            SensorDataQueue->enableSensor((*Begin).m_lp_sensor->getHandle(),
                                         Delay, Batch, IsFiFo);
            printf("Enabled Sensor: %s \n",
                   (*Begin).m_lp_sensor->getName().string());
        }
    }
}


static void DisableSelectedSensor(SensorCollection& AllSensor,
                                  sp<SensorEventQueue> SensorDataQueue)
{
    int err;
    SensorCollection::iterator Begin, End;

    End = AllSensor.end();
    for(Begin = AllSensor.begin(); Begin != End; ++Begin) {
        if ((*Begin).m_IsEnable) {
            err = SensorDataQueue->disableSensor((*Begin).m_lp_sensor);

            if (err != NO_ERROR) {
                printf("disableSensor() for '%s'failed, ret err = %d\n",
                       (*Begin).m_lp_sensor->getName().string(), err);
            } else {
                printf("Disable Sensor: %s \n",
                       (*Begin).m_lp_sensor->getName().string());
            }
        }
    }
}


int main(int argc, char** argv)
{
    int err;

    char const* action = "measure";
    int delay = 20, sample = -1, verbose = 0, show_list = 0;
    int batch_time_ms = 0, wakeup=0;

    sp<SensorThread> sensor_thread;
    SensorManager& mgr(SensorManager::getInstance());
    sp<SensorEventQueue> queue = mgr.createEventQueue();

    SensorCollection AllSensor;
    std::vector<std::string> sensor_name_list;

    Sensor const* const* list;
    int count;

    int incalibrate = 0;

    if (argc < 2) {
        DisplaySensorParameterName();
        return -1;
    }

    if (queue == NULL) {
        printf("createEventQueue returned NULL\n");
        return 0;
    } else {
        count = mgr.getSensorList(&list);
        SensorList2SensorCollection(AllSensor, list, count);
    }

    while (1) {
        int c = getopt_long(argc, argv,
                            short_options, long_options, NULL);

        if (c == -1) {
            break;
        }

        switch (c) {
        case 'a':
            action = optarg;
            break;
        case 'd':
            delay = atoi(optarg);
            break;
        case 's':
            sample = atoi(optarg);
            break;
        case 'v':
            verbose = 1;
            break;
        case 'l':
            show_list = 1;
            break;
        case 'b':
            batch_time_ms = atoi(optarg);
            break;
        case 'w':
            wakeup = 1;
            break;
        default:
            return 0;
        }
    }

    /* if list is set, just put the sensor list info */
    if (show_list) {
#if 0
        printf("%d sensors found:\n", count);
        for (int i=0 ; i<count ; i++) {
            if(list[i] == NULL )
                break;
            
            printf("index: %d, %-32s| vendor: %-28s | handle: %10d | type: %5d | name: %s\n",
                   i,
                   list[i]->getName().string(),
                   list[i]->getVendor().string(),
                   list[i]->getHandle(),
                   list[i]->getType(),
                   getSensorName(list[i]->getType()));
        }
#else
        DumpExistSensor(AllSensor);
#endif
        return 0;
    }

    if (strcmp(action, "set") == 0) {
        printf("'set' is not supported\n");
        return 0;
    } else if (strcmp(action, "get") == 0) {
        printf("'get' is not supported\n");
        return 0;
    } else if (strcmp(action, "selftest") == 0) {
        printf("'selftest' is not supported\n");
        return 0;
    } else if ((strcmp(action, "measure") == 0)
             || (strcmp(action, "calibrate") == 0)) {
		/* just open 1 sensor for easy test */
        /*int type;*/
        int numSkipSensors = 0;
        Sensor const *sensor = NULL;

		/*
		   if(optind >= argc) {
			   printf("invalid cmd parameters\n");
			   return 0;
		   }
		   if ((type = getSensorType(argv[optind])) == -1) {
			   printf("invalid sensor type (%s)\n", argv[optind]);
			   return 0;
		   }
		   */

		/*
		   Save all request sensor name to vector
		   */
        for (optind; optind < argc; ++optind) {
            printf("request Sensor type string name: (%s)\n", argv[optind]);
            sensor_name_list.push_back(argv[optind]);
        }

        /*
          Select the sensor which request by parameters
         */
        if (!SelectRequestSensor(AllSensor,
                                 sensor_name_list)) {
            printf("No Sensor want to been enable.\n");
            return -1;
        }

        if (strcmp(action, "calibrate") == 0) {
            sample = -1;
            //incalibrate =1;
            incalibrate =0;
        }

        sensor_thread = new SensorThread(queue, sample, incalibrate);
        if (sensor_thread == NULL) {
            printf("failed to create sensor thread\n");
            return 0;
        }

        if (incalibrate == 0) {
            sensor_thread->run("sensor-loop", PRIORITY_BACKGROUND);
        }
        
#if 0
        // For the following sensor types, return a wake-up sensor. These types are by default
        // defined as wake-up sensors. For the rest of the sensor types defined in sensors.h return
        // a non_wake-up version.
        if (type == SENSOR_TYPE_PROXIMITY
            || type == SENSOR_TYPE_SIGNIFICANT_MOTION
            || type == SENSOR_TYPE_TILT_DETECTOR
            || type == SENSOR_TYPE_WAKE_GESTURE
            || type == SENSOR_TYPE_GLANCE_GESTURE
            || type == SENSOR_TYPE_PICK_UP_GESTURE) {
            wakeup = true;
        }

        for (int i=0 ; i<count ; i++) {
            if(list[i] == NULL)
                break;

            if (list[i]->getType() == type &&
                list[i]->isWakeUpSensor() == wakeup) {
                sensor = list[i];
                break;
            }
        }

        if (sensor == NULL) {
            printf("get sensor of type:%d error\n", type);
            return 0;
        }
#endif


#if 0
        if (queue->enableSensor(sensor->getHandle(), ns2us(ms2ns(delay)),
                                ns2us(ms2ns(batch_time_ms)), wakeup ? SENSORS_BATCH_WAKE_UPON_FIFO_FULL : 0) != NO_ERROR) {
            printf("enable sensor of type %d error\n", type);
            return 0;
        }
#else

        EnableSelectedSensor(AllSensor, queue,
                             delay, batch_time_ms);
#endif


#if 0
        if (incalibrate == 1) {
            err = queue->calibrateSensor(sensor->getHandle());
            if (err) {
                printf("force calibrate sensor of type %d error %d\n", type, err);
            }

            if (type == SENSOR_TYPE_MAGNETIC_FIELD || type == SENSOR_TYPE_ORIENTATION)
                sensor_thread->run("sensor-loop", PRIORITY_BACKGROUND);
            else {
                if (!err) {
                    printf("calibrate successful type %d\n", type);
                }
                
                return 0;
            }
        }
#endif
        sensor_thread->join();
        if (verbose) {
            printf("sensor thread terminated\n");
        }
        
        DisableSelectedSensor(AllSensor, queue);
    } else {
        printf("unknown action %s\n", action);
        return 0;
    }

    return 0;
}
