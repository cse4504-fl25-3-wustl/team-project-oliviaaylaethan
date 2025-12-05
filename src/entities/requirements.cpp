#include "requirements.h"
#include <algorithm>

Requirements::Requirements() : jobSiteLocation_(""), clientName_(""), acceptsPallets_(std::nullopt), acceptsCrates_(std::nullopt), hasLoadingDock_(std::nullopt), needsLiftgate_(std::nullopt), needsInsideDelivery_(std::nullopt), serviceType_("") {}

std::string Requirements::getJobSiteLocation() const {
    return jobSiteLocation_;
}

std::string Requirements::getClientName() const {
    return clientName_;
}

std::optional<bool> Requirements::getAcceptsPallets() const {
    return acceptsPallets_;
}

std::optional<bool> Requirements::getAcceptsCrates() const {
    return acceptsCrates_;
}

std::optional<bool> Requirements::getHasLoadingDock() const {
    return hasLoadingDock_;
}

std::optional<bool> Requirements::getNeedsLiftgate() const {
    return needsLiftgate_;
}

std::optional<bool> Requirements::getNeedsInsideDelivery() const {
    return needsInsideDelivery_;
}

std::string Requirements::getServiceType() const {
    return serviceType_;
}

std::optional<bool> Requirements::convertStringToOptional(std::string answer) {
    std::transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
    if (answer == "y") {
        return true;
    } else if (answer == "n") {
        return false;
    } else {
        return std::nullopt;
    }
}

std::string Requirements::convertOptionalToString(std::optional<bool> answer) {
    if (answer.has_value()) {
        if (answer.value()) {
            return "Yes";
        } else {
            return "No";
        }
    }
    return "N/A";  
}

void Requirements::setJobSiteLocation(std::string location) {
    jobSiteLocation_ = location;
}

void Requirements::setClientName(std::string clientName) {
    clientName_ = clientName;
}

void Requirements::setAcceptsPallets(std::string answer) {
    acceptsPallets_ = convertStringToOptional(answer);
}

void Requirements::setAcceptsCrates(std::string answer) {
    acceptsCrates_ = convertStringToOptional(answer);
}

void Requirements::setHasLoadingDock(std::string answer) {
    hasLoadingDock_ = convertStringToOptional(answer);
}

void Requirements::setNeedsLiftgate(std::string answer) {
    needsLiftgate_ = convertStringToOptional(answer);
}

void Requirements::setNeedsInsideDelivery(std::string answer) {
    needsInsideDelivery_ = convertStringToOptional(answer);
}

void Requirements::setServiceType(std::string serviceType) {
    serviceType_ = serviceType;
}