DEFAULT_TITLE = "Control Interface"

NAV_BAR = [
    {
        'id': 'dashboard',
        'href': '/dashboard',
        'title': 'Vehicle Dashboard',
        'icon': 'pe-7s-rocket'
    },
    {
        'id': 'dts',
        'href': 'dts',
        'title': 'Dynamic Test Stand',
        'icon': 'pe-7s-joy'
    },
    {
        'id': 'proofTest',
        'href': 'proofTest',
        'title': 'Proof Test',
        'icon': 'pe-7s-gym'
    },
    {
        'id': 'battery',
        'href': 'ui/battery',
        'title': 'Batteries',
        'icon': 'pe-7s-battery'
    },
    {
        'id': 'profile',
        'href': 'profile',
        'title': 'Flight Profile',
        'icon': 'pe-7s-user'
    },
    {
        'id': 'feeds',
        'href': 'ui/feeds',
        'title': 'Live Streams',
        'icon': 'pe-7s-video'
    },
]
NAV_IDS = [x['id'] for x in NAV_BAR]


def get_page_title(page):
    if page in NAV_IDS:
        title = NAV_BAR[NAV_IDS.index(page)]['title']
    else:
        title = DEFAULT_TITLE
    return title