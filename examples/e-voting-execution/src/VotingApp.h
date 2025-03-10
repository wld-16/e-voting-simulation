//
// Created by wld on 27.11.22.
//

#ifndef E_VOTING_APP_H
#define E_VOTING_APP_H

#include "inet/common/INETDefs.h"
#include <inet/applications/tcpapp/TcpSessionApp.h>
#include <inet/common/socket/SocketMap.h>
#include "network/connectionService.h"
#include "inet/applications/tcpapp/TcpServerSocketIo.h"
#include "evoting/peer.h"
#include "network/syncService.h"
#include "../../../src/inetSocketAdapter.h"
#include "evoting/service/distributionService.h"
#include "evoting/service/tallyService.h"
#include "evoting/service/electionService.h"

namespace voting {
    class VotingApp : public inet::TcpAppBase {
        inet::cMessage *createElectionSelfMessage;
        inet::cMessage *placeVoteSelfMessage;
        inet::cMessage *receive3PReqestSelfMessage;
        inet::cMessage *forwardDirectionRequestSelfMessage;
        inet::cMessage *publishSelfMessage;
        inet::cMessage *hopsSelfMessage;
        inet::cMessage *directionSelfMessage;
        inet::cMessage *subscribeSelfMessage;
        inet::cMessage *closePublishMessage;
        inet::cMessage *closeSubscribeMessage;
        inet::cMessage *tTallyAtMessage;
        inet::cMessage *tConfirmAtMessage;
        inet::cMessage *tRequestKeysAtMessage;
        inet::cMessage *closeSetupSelfMessage;
        std::vector<inet::cMessage*> tKeyMessages;
        std::vector<omnetpp::cMsgPar*> addressPars;
        bool isReceiving = false;
        bool isInitializingDirectionDistribution = false;
        bool doesConnect = true;

        bool is_receiving_multipackage_message = false;
        int exit_sequence_count = 0;
        bool has_received_last_package_from_multi_message = false;
        uint8_t saved_package_type = 0;
        uint8_t last_received_package_type = 0;
        std::stringstream multi_message_stream;

        connectionService connection_service;
        distributionService distribution_service;
        hillEncryptionService hill_encryption_service;
        tallyService* tally_service;
        electionService* election_service;

        inetSocketAdapter socket_up_adapter;
        inetSocketAdapter socket_down_adapter;
        inetSocketAdapter socket_no_direction_adapter;
        inetSocketAdapter subscribe_socket_adapter;
        inetSocketAdapter publish_socket_adapter;
        inetSocketAdapter request_keys_socket;

        inet::TcpSocket* publish_socket = new inet::TcpSocket();
        inet::TcpSocket* subscribe_socket = new inet::TcpSocket();

        inet::TcpSocket* request_up_socket = new inet::TcpSocket();
        inet::TcpSocket listen_socket;
        inet::TcpSocket* request_down_socket = new inet::TcpSocket();

        inet::TcpSocket* request_key_socket = new inet::TcpSocket();

        inet::TcpSocket reply_key_socket;

        std::string address_up, address_down = "";
        int publish_port, subscribe_port;
        std::vector<election> election_box;

        // For tallying
        std::map<size_t, std::queue<std::string>> election_keys_to_send;
        std::map<size_t, std::string> own_election_keys;
        std::map<size_t, std::vector<std::string>> received_election_keys;

        std::map<size_t, bool> is_evaluated_votes_map;

        size_t position = 0;

        std::string sendTowards = "";
        std::string local_address = "";

        omnetpp::cMsgPar *nextDirection;
        omnetpp::cMsgPar *received3PData;

        // TODO: Migrate to use peer
        std::set<std::string> nodes;
        std::map<std::string, std::string> connection_map;
        std::string nodesString;


        int request_socket_id = 0;

        size_t current_hops = 0;
        std::string received_from_direction;

        inet::SocketMap socketMap;

        double pauseBeforePublish = 0.1;
        double pauseBeforeSendHops = 0.0;
        double pause_before_close_publish = 0.1;
        double pause_before_send_initial_direction = 0.1;
        double pauseBeforeForwardPortsRequest = 0.2;
        double pause_before_send_direction_forward = 0.2;

        void handleTimer(inet::cMessage *msg) override;
        void handleCrashOperation(inet::LifecycleOperation *) override;
        void handleStopOperation(inet::LifecycleOperation *) override;
        void handleStartOperation(inet::LifecycleOperation *) override;
        void handleMessageWhenUp(inet::cMessage *msg) override;
        void socketAvailable(inet::TcpSocket *socket, inet::TcpAvailableInfo *availableInfo) override;
        void socketEstablished(inet::TcpSocket *socket) override;
        void socketStatusArrived(inet::TcpSocket *socket, inet::TcpStatusInfo *status) override;
        void socketDataArrived(inet::TcpSocket *socket, inet::Packet *msg, bool urgent) override;
        void initialize(int stage) override;

    public:
        void writeStateToFile(std::string file);

    private:
        std::string received_sync_request_from;
        void initElectionDistribution(election& election);
        void printSocketMap();
    };
}

#endif //E_VOTING_APP_H
