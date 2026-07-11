My pattern is STATE.

MembershipLevel is the context object.
It owns a raw pointer to the current MembershipState.

Normal, Silver, Gold, and VIP are the concrete states.
The work happens through the current state, and MembershipLevel changes that
state when automatic point-based upgrade or downgrade rules require it.
