DEFAULT_TITLE = "Control Interface"

NAV_BAR = [
    {
        'id': 'dashboard',
        'href': '',
        'title': 'Vehicle Dashboard',
        'icon': 'pe-7s-rocket'
    },
    {
        'id': 'profile',
        'href': 'profile',
        'title': 'Connect To Pod',
        'icon': 'pe-7s-user'
    }
]
NAV_IDS = [x['id'] for x in NAV_BAR]


def get_page_title(page):
    if page in NAV_IDS:
        title = NAV_BAR[NAV_IDS.index(page)]['title']
    else:
        title = DEFAULT_TITLE
    return title