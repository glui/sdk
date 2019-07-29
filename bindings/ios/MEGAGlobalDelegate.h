/**
 * @file MEGAGlobalDelegate.h
 * @brief Delegate to get global events
 *
 * (c) 2013-2014 by Mega Limited, Auckland, New Zealand
 *
 * This file is part of the MEGA SDK - Client Access Engine.
 *
 * Applications using the MEGA API must present a valid application key
 * and comply with the the rules set forth in the Terms of Service.
 *
 * The MEGA SDK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * @copyright Simplified (2-clause) BSD License.
 *
 * You should have received a copy of the license along with this
 * program.
 */
#import <Foundation/Foundation.h>

/**
 * @brief Protocol to get information about global events.
 *
 * You can implement this interface and start receiving events calling [MEGASdk addMEGAGlobalDelegate:].
 *
 * MEGADelegate objects can also receive global events.
 */
@protocol MEGAGlobalDelegate <NSObject>

@optional

/**
 * @brief This function is called when there are new or updated contacts in the account.
 *
 * @param api MEGASdk object connected to the account.
 * @param userList List that contains the new or updated contacts.
 */
- (void)onUsersUpdate:(MEGASdk *)api userList:(MEGAUserList *)userList;

/**
 * @brief This function is called when there are new or updated user alerts in the account
 *
 * The SDK retains the ownership of the MEGAUserAlertList in the second parameter. The list and all the
 * MEGAUserAlert objects that it contains will be valid until this function returns. If you want to save the
 * list, use [MEGAUserAlertList clone]. If you want to save only some of the MEGAUserAlert objects, use [MEGAUserAlert clone]
 * for those objects.
 *
 * @param api MEGASdk object connected to the account
 * @param userAlertList List that contains the new or updated contacts
 */
- (void)onUserAlertsUpdate:(MEGASdk *)api userAlertList:(MEGAUserAlertList *)userAlertList;

/**
 * @brief This function is called when there are new or updated nodes in the account.
 *
 * When the full account is reloaded or a large number of server notifications arrives at once, the
 * second parameter will be nil.
 *
 * @param api MEGASdk object connected to the account.
 * @param nodeList List that contains the new or updated nodes.
 */
- (void)onNodesUpdate:(MEGASdk *)api nodeList:(MEGANodeList *)nodeList;

/**
 * @brief This function is called when the account has been updated (confirmed/upgraded/downgraded)
 *
 * The usage of this delegate to handle the external account confirmation is deprecated.
 * Instead, you should use [MEGAGlobalDelegate onEvent:event:].
 *
 * @param api MEGASdk object connected to the account
 */
- (void)onAccountUpdate:(MEGASdk *)api;

/**
 * @brief This function is called when there are new or updated contact requests in the account
 *
 * When the full account is reloaded or a large number of server notifications arrives at once, the
 * second parameter will be nil.
 *
 * @param api MEGASdk object connected to the account
 * @param contactRequestList List that contains the new or updated contact requests
 */
- (void)onContactRequestsUpdate:(MEGASdk *)api contactRequestList:(MEGAContactRequestList *)contactRequestList;

/**
 * @brief This function is called when an inconsistency is detected in the local cache.
 *
 * You should call [MEGASdk fetchNodes] when this callback is received.
 *
 * @param api MEGASdk object connected to the account.
 */
- (void)onReloadNeeded:(MEGASdk *)api;

/**
 * The details about the event, like the type of event and optionally any
 * additional parameter, is received in the \c params parameter.
 *
 * You can check the type of event by calling [MEGAEvent type]
 *
 * Currently, the following type of events are notified:
 *
 *  - EventCommitDB: when the SDK commits the ongoing DB transaction.
 *  This event can be used to keep synchronization between the SDK cache and the
 *  cache managed by the app thanks to the sequence number.
 *
 *  Valid data in the MegaEvent object received in the callback:
 *      - [MEGAEvent text]: sequence number recorded by the SDK when this event happened
 *
 *  - EventAccountConfirmation: when a new account is finally confirmed
 *  by the user by confirming the signup link.
 *
 *  Valid data in the MegaEvent object received in the callback:
 *      - [MEGAEvent text]: email address used to confirm the account
 *
 *  - EventChangeToHttps: when the SDK automatically starts using HTTPS for all
 *  its communications. This happens when the SDK is able to detect that MEGA servers can't be
 *  reached using HTTP or that HTTP communications are being tampered. Transfers of files and
 *  file attributes (thumbnails and previews) use HTTP by default to save CPU usage. Since all data
 *  is already end-to-end encrypted, it's only needed to use HTTPS if HTTP doesn't work. Anyway,
 *  applications can force the SDK to always use HTTPS using MegaApi::useHttpsOnly. It's recommended
 *  that applications that receive one of these events save that information on its settings and
 *  automatically enable HTTPS on next executions of the app to not force the SDK to detect the problem
 *  and automatically switch to HTTPS every time that the application starts.
 *
 * - EventDisconnect: when the SDK performs a disconnect to reset all the
 * existing open-connections, since they have become unusable. It's recommended that the app
 * receiving this event reset its connections with other servers, since the disconnect
 * performed by the SDK is due to a network change or IP addresses becoming invalid.
 *
 * - EventAccountBlocked: when the account get blocked, typically because of
 * infringement of the Mega's terms of service repeatedly. This event is followed by an automatic
 * logout.
 *
 *  Valid data in the MegaEvent object received in the callback:
 *      - [MEGAEvent text]: message to show to the user.
 *      - [MEGAEvent number]: code representing the reason for being blocked.
 *          200: suspension message for any type of suspension, but copyright suspension.
 *          300: suspension only for multiple copyright violations.
 *          400: the subuser account has been disabled.
 *          401: the subuser account has been removed.
 *
 * - EventStorage: when the status of the storage changes.
 *
 * For this event type, MegaEvent::getNumber provides the current status of the storage
 *
 * There are three possible storage states:
 *     - StorageStateGreen = 0
 *     There are no storage problems
 *
 *     - StorageStateOrange = 1
 *     The account is almost full
 *
 *     - StorageStateRed = 2
 *     The account is full. Uploads have been stopped
 *
 *     - StorageStateChange = 3
 *     There is a possible significant change in the storage state.
 *     It's needed to call [MEGASdk getAccountDetails] to check the storage status.
 *     After calling it, this callback will be called again with the corresponding
 *     state if there is really a change.
 *
 * - EventNodesCurrent: when all external changes have been received
 *
 * - EventMediaInfoReady: when codec-mappings have been received
 *
 * - EventBusinessStatus: when the status of a business account has changed.
 * The posible values are:
 *   - BusinessStatusExpired = -1
 *   - BusinessStatusInactive = 0
 *   - BusinessStatusActive = 1
 *   - BusinessStatusGracePeriod = 2
 *
 *  Valid data in the MEGAEvent object received in the callback:
 *    - [MEGAEvent number] returns the new business status.
 *
 *
 * @param api MEGASdk object connected to the account
 * @param event Details about the event
 */
- (void)onEvent:(MEGASdk *)api event:(MEGAEvent *)event;

@end
