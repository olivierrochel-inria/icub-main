// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
* Copyright (C) 2007 Robotcub Consortium
* CopyPolicy: Released under the terms of the GNU GPL v2.0.
*
*/

#ifndef __GTKMM_ICUB_INTERFACE_GUI_SERVER_H__
#define __GTKMM_ICUB_INTERFACE_GUI_SERVER_H__

#include "iCubNetwork.h"
#include <yarp/os/Port.h>
#include <yarp/os/Semaphore.h>

class JointRemapper
{
public:
    JointRemapper(){}
    ~JointRemapper(){}

    void push(int d0,int d1,int j0)
    {
        mapD0.push_back(d0);
        mapD1.push_back(d1);
        mapJ0.push_back(j0);
    }
    
    int getJoint(int pos)
    {
        pos*=2;

        for (int i=0; i<(int)mapD0.size(); ++i)
        {
            if (pos>=mapD0[i] && pos<=mapD1[i])
            {
                return mapJ0[i]+pos-mapD0[i];
            }
        }

        return -1;
    }

protected:
    std::vector<int> mapD0,mapD1,mapJ0;
};

class iCubInterfaceGuiServer : public yarp::os::Thread
{
public:
    iCubInterfaceGuiServer(yarp::os::Property &robot)
    {
        std::vector<JointRemapper> jointRmp;

        yarp::os::ConstString robotName=robot.find("name").asString();

        yarp::os::Bottle general=robot.findGroup("GENERAL");
        yarp::os::Bottle *parts=general.find("parts").asList();

        for (int t=0; t<parts->size(); ++t)
        {
            yarp::os::ConstString partName=parts->get(t).asString();
            yarp::os::Bottle part=robot.findGroup(partName.c_str());
            
            yarp::os::Bottle *networks=part.find("networks").asList();

            for (int n=0; n<networks->size(); ++n)
            {
                std::string netName(networks->get(n).asString().c_str());

                yarp::os::Bottle jMap=part.findGroup(netName.c_str());

                int j0=jMap.get(1).asInt();
                int d0=jMap.get(3).asInt();
                int d1=jMap.get(4).asInt();

                yarp::os::Bottle net=robot.findGroup(netName.c_str());

                /*
                addNetwork(std::string(netName.c_str()),
                           std::string(net.find("file").asString().c_str()),
                           std::string(net.find("device").asString().c_str()),
                           std::string(net.find("canbusdevice").asString().c_str()),
                           part.find("threadrate").asInt());
                */

                bool bExists=false;

                for (unsigned int i=0; i<mNetworks.size(); ++i)
                {
                    if (mNetworks[i]->mName==netName)
                    {
                        jointRmp[i].push(d0,d1,j0);
                        bExists=true;
                        break;
                    }
                }

                if (!bExists)
                {
                    mNetworks.push_back(new iCubNetwork(netName,
                                                        std::string(net.find("file").asString().c_str()),
                                                        std::string(net.find("device").asString().c_str()),
                                                        std::string(net.find("canbusdevice").asString().c_str()),
                                                        part.find("threadrate").asInt()));

                    jointRmp.push_back(JointRemapper());
                    jointRmp.back().push(d0,d1,j0);
                }
            }
        }

        // we have now the networks list

        for (unsigned int n=0; n<mNetworks.size(); ++n)
        {
            yarp::os::Property netConf;
            netConf.fromConfigFile(mNetworks[n]->mFile.c_str());
            yarp::os::Bottle canConf=netConf.findGroup("CAN");

            mNetworks[n]->mID=canConf.find("CanDeviceNum").asInt();
        
            yarp::os::Bottle devices=canConf.findGroup("CanAddresses");
            
            for (int d=1; d<devices.size(); ++d)
            {
                int joint=jointRmp[n].getJoint(d-1);
                iCubBoard *bll=new iCubBLLBoard(devices.get(d).asInt(),joint,joint+1);
                mNetworks[n]->addBoard(bll);
            }
        }

        mPort.open((robotName+"/gui").c_str());
    }

    virtual ~iCubInterfaceGuiServer()
    {
    }

    void addNetwork(iCubNetwork* net)
    {
        for (unsigned int i=0; i<mNetworks.size(); ++i)
        {
            if (*mNetworks[i]==*net)
            {
                delete net;
                return;
            }
        }

        mNetworks.push_back(net);
    }

    void run()
    {
    }
    /*
        yarp::os::Bottle msg;
        yarp::os::Bottle rpl;

        while (!isStopping())
        {
            m_port.read(msg,true);
            yarp::os::ConstString cmd=msg.get(0).asString();

            if (cmd=="get_conf")
            {
                rpl.clear();
                m_mutex.wait();
                for (unsigned int n=0; n<m_networks.size(); ++n)
                {
                    // ...
                    // tutto cambiato
                    // ...
                }    
                m_mutex.post();

                m_port.reply(rpl);
            }
            else if (cmd=="get_data")
            {
                m_mutex.wait();
                
                // ...
                // tutto cambiato
                // ...

                m_mutex.post();
            }
        } 
    }

    yarp::os::Bottle toBottle();

    bool write(std::string& address,std::string& data)
    {
        m_mutex.wait();
        for (unsigned int n=0; n<m_networks.size(); ++n)
        {
            if (m_networks[n]->write(address,data)
            {
                m_mutex.post();
                return true;
            }
        }
        m_mutex.post();
        return false;
    }
    */

protected:
    yarp::os::Port mPort;
    yarp::os::Semaphore mMutex;
    std::vector<iCubNetwork*> mNetworks;
};

#endif //__GTKMM_ICUB_INTERFACE_GUI_H__
