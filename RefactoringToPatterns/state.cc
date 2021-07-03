
#include <assert.h>
#include <stdlib.h>

#include <iostream>
#include <memory>
#include <string>

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

class SystemPermission;

enum StateType {
    REQUESTED,
    CLAIMED,
    GRANTED,
    DENIED,
    UNIX_REQUESTED,
    UNIX_CLAIMED
};

class PermissionState {
public:
    PermissionState(const std::string& name) : name_(name) {}

    virtual ~PermissionState() {}

    const std::string& ToString() const {
        return name_;
    }

    virtual void deniedBy(SystemAdmin admin, SystemPermission* permission) {}

    virtual void grantedBy(SystemAdmin admin, SystemPermission* permission) {}

    virtual void claimedBy(SystemAdmin admin, SystemPermission* permission) {}

private:
    std::string name_;
};

class PermissionRequested : public PermissionState {
public:
    PermissionRequested() : PermissionState("PermissionRequested") {}

    void claimedBy(SystemAdmin admin, SystemPermission* permission) override;
};

class PermissionClaimed : public PermissionState {
public:
    PermissionClaimed() : PermissionState("PermissionClaimed") {}

    void deniedBy(SystemAdmin admin, SystemPermission* permission) override;

    void grantedBy(SystemAdmin admin, SystemPermission* permission) override;
};

static PermissionState* CreatePermissionState(StateType state_type) {
    switch (state_type) {
    case StateType::REQUESTED:
    case StateType::UNIX_REQUESTED:
        return new PermissionRequested();
    case StateType::CLAIMED:
    case StateType::GRANTED:
    case StateType::UNIX_CLAIMED:
        return new PermissionClaimed();
    default:
        return nullptr;
    }
    return nullptr;
}

class SystemPermission {
public:
    SystemPermission(SystemProfile profile) :
        profile_(profile) {
            state_.reset(CreatePermissionState(StateType::REQUESTED));
        }

    void claimedBy(SystemAdmin admin) {
        state_->claimedBy(admin, this);
    }

    void grantedBy(SystemAdmin admin) {
        state_->grantedBy(admin, this);
    }

    void deniedBy(SystemAdmin admin) {
        state_->deniedBy(admin, this);
    }

    void setState(PermissionState* state) {
        state_.reset(state);
    }

    void setIsGranted(bool is_granted) {
        is_granted_ = is_granted;
    }

    void setIsUnixPermissionGranted(bool is_unix_permission_granted) {
        is_unix_permission_granted_ = is_unix_permission_granted;
    }

    bool isUnixPermissionGranted() {
        return is_unix_permission_granted_;
    }

    PermissionState* getState() const {
        return state_.get();
    }

    SystemAdmin getAdmin() {
        return admin_;
    }

    SystemProfile getProfile() const {
        return profile_;
    }

    bool isGranted() const {
        return is_granted_;
    }

    void notifyUserOfPermissionRequestResult() {
        std::cout << "state=" << state_->ToString()
            << ", admin=" << admin_.Name()
            << ", is_granted=" << is_granted_
            << ", is_unix_permission_granted="
            << is_unix_permission_granted_ << std::endl;;
    }

    void willBeHandleBy(SystemAdmin admin) {
        admin_ = admin;
        std::cout << admin_.ToString() << std::endl;
    }

private:
    std::unique_ptr<PermissionState> state_;
    SystemAdmin admin_;
    SystemProfile profile_;

    bool is_granted_ = false;
    bool is_unix_permission_granted_ = false;
};

void PermissionRequested::claimedBy(SystemAdmin admin, SystemPermission* permission) {
    permission->willBeHandleBy(admin);
    permission->setState(CreatePermissionState(StateType::CLAIMED));
}

void PermissionClaimed::deniedBy(SystemAdmin admin, SystemPermission* permission) {
    if (permission->getAdmin() != admin) {
        return;
    }

    permission->setIsGranted(false);
    permission->setIsUnixPermissionGranted(false);
    permission->setState(CreatePermissionState(StateType::DENIED));
    permission->notifyUserOfPermissionRequestResult();
}

void PermissionClaimed::grantedBy(SystemAdmin admin, SystemPermission* permission) {
    if (permission->getAdmin() != admin) {
        return;
    }

    if (permission->getProfile().isUnixPermissionRequired()
            && !permission->isUnixPermissionGranted()) {
        permission->setState(CreatePermissionState(StateType::UNIX_REQUESTED));
        permission->notifyUserOfPermissionRequestResult();
        return;
    }
    permission->setState(CreatePermissionState(StateType::GRANTED));
    permission->setIsGranted(true);
    permission->notifyUserOfPermissionRequestResult();
}

int main() {
    SystemPermission permission = SystemPermission(SystemProfile());
    SystemAdmin admin;
    permission.setState(CreatePermissionState(StateType::REQUESTED));
    permission.grantedBy(admin);
    assert(!permission.isGranted());

    permission.setState(CreatePermissionState(StateType::CLAIMED));
    permission.claimedBy(admin);
    permission.grantedBy(admin);
    assert(permission.isGranted());
}
