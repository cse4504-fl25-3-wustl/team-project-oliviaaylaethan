#pragma once

#include <string>
#include <optional>

class Requirements {
private:
    std::string jobSiteLocation_;
    std::string clientName_;
    std::optional<bool> acceptsPallets_; // true for YES, false for NO, std::nullopt for N/A
    std::optional<bool> acceptsCrates_;
    std::optional<bool> hasLoadingDock_;
    std::optional<bool> needsLiftgate_;
    std::optional<bool> needsInsideDelivery_;
    std::string serviceType_;
    std::optional<bool> convertStringToOptional(std::string answer);

public:
    Requirements();

    // CSV order/index to Field
    enum RequirementIndex {
        JOB_SITE_LOCATION,
        CLIENT_NAME,
        ACCEPTS_PALLETS,
        ACCEPTS_CRATES,
        HAS_LOADING_DOCK,
        NEEDS_LIFTGATE,
        NEEDS_INSIDE_DELIVERY,
        SERVICE_TYPE
    };

    // Utilities
    static std::string convertOptionalToString(std::optional<bool> answer);

    // Getters
    std::string getJobSiteLocation() const;
    std::string getClientName() const;
    std::optional<bool> getAcceptsPallets() const;
    std::optional<bool> getAcceptsCrates() const;
    std::optional<bool> getHasLoadingDock() const;
    std::optional<bool> getNeedsLiftgate() const;
    std::optional<bool> getNeedsInsideDelivery() const;
    std::string getServiceType() const;

    // Setters
    void setJobSiteLocation(std::string location);
    void setClientName(std::string clientName);
    void setAcceptsPallets(std::string answer);
    void setAcceptsCrates(std::string answer);
    void setHasLoadingDock(std::string answer);
    void setNeedsLiftgate(std::string answer);
    void setNeedsInsideDelivery(std::string answer);
    void setServiceType(std::string serviceType);
};