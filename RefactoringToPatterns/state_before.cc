
#include <assert.h>
#include <stdlib.h>

#include <iostream>
#include <string>

const std::string REQUESTED = "REQUESTED";
const std::string CLAIMED = "CLAIMED";
const std::string GRANTED = "GRANTED";
const std::string DENIED = "DENIED";
const std::string UNIX_REQUESTED = "UNIX_REQUESTED";
const std::string UNIX_CLAIMED = "UNIX_CLAIMED";

struct SystemProfile {
    bool isUnixPermissionRequired() {
        return unix_permission_granted_;
    }

    bool unix_permission_granted_ = false;
};

struct SystemAdmin {
    SystemAdmin() : name_("default") {}

    SystemAdmin(const std::string& name) : name_(name) {}

    std::string ToString() {
        return "system admin";
    }

    bool operator==(const SystemAdmin& radmin) {
        name_ == radmin.Name();
    }

    bool operator!=(const SystemAdmin& radmin) {
        return !(*this==radmin);
    }

    const std::string& Name() const {
        return name_;
    }

    std::string name_;
};

class SystemPermission {
public:
    void claimedBy(SystemAdmin admin);

    void grantedBy(SystemAdmin admin);

    void deniedBy(SystemAdmin admin);

    std::string state() const {
        return state_;
    }

    bool isGranted() const {
        return is_granted_;
    }

    void notifyUserOfPermissionRequestResult() {
        std::cout << "state=" << state_
            << ", admin=" << admin_.Name()
            << ", is_granted=" << is_granted_
            << ", is_unix_permission_granted="
            << is_unix_permission_granted_ << std::endl;;
    }
private:
    void willeBeHandleBy(SystemAdmin admin) {
        std::cout << admin.ToString() << std::endl;
    }

private:
    std::string state_ = REQUESTED;
    SystemAdmin admin_;
    SystemProfile profile_;

    bool is_granted_ = false;
    bool is_unix_permission_granted_ = false;
};

void SystemPermission::claimedBy(SystemAdmin admin) {
    if (state_ != REQUESTED && state_ != UNIX_REQUESTED) {
        abort();
    }

    willeBeHandleBy(admin);

    if (state_ == REQUESTED) {
        state_ = CLAIMED;
    } else if (state_ == UNIX_REQUESTED) {
        state_ = UNIX_CLAIMED;
    }
}

void SystemPermission::deniedBy(SystemAdmin admin) {
    if (state_ == CLAIMED && state_ == UNIX_CLAIMED) {
        abort();
    }

    if (admin_ != admin) {
        return;
    }

    is_granted_ = false;
    is_unix_permission_granted_ = false;
    state_ = DENIED;
    notifyUserOfPermissionRequestResult();
}

void SystemPermission::grantedBy(SystemAdmin admin) {
    if (state_ != CLAIMED && state_ != UNIX_REQUESTED) {
        return;
    }

    if (admin_ != admin) {
        return;
    }

    if (profile_.isUnixPermissionRequired() && state_ == UNIX_REQUESTED) {
        is_unix_permission_granted_ = true;
    } else if (profile_.isUnixPermissionRequired() && !is_unix_permission_granted_) {
        state_ = UNIX_REQUESTED;
        notifyUserOfPermissionRequestResult();
        return;
    }
    state_ = GRANTED;
    is_granted_ = true;
    notifyUserOfPermissionRequestResult();
}

int main() {
    SystemPermission permission;
    SystemAdmin admin;
    permission.grantedBy(admin);
    assert(permission.state() == REQUESTED);
    assert(!permission.isGranted());

    permission.claimedBy(admin);
    permission.grantedBy(admin);
    assert(permission.state() == GRANTED);
    assert(permission.isGranted());
}
