// Copyright (C) Explorer++ Project
// SPDX-License-Identifier: GPL-3.0-only
// See LICENSE in the top level directory

#pragma once

#include "ShellBrowser/ViewModes.h"
#include "Tab.h"
#include "TabContainerInterface.h"
#include "../ThirdParty/Sol/sol.hpp"
#include <boost/signals2.hpp>
#include <unordered_map>

namespace Plugins
{
	class TabsApi
	{
	public:
		struct Tab
		{
			int id;
			std::wstring location;
			std::wstring name;
			ViewMode viewMode;
			bool locked;
			bool addressLocked;

			Tab(const ::Tab &tabInternal)
			{
				TCHAR path[MAX_PATH];
				tabInternal.shellBrower->QueryCurrentDirectory(SIZEOF_ARRAY(path), path);

				id = tabInternal.id;
				location = path;
				name = tabInternal.szName;
				viewMode = tabInternal.shellBrower->GetCurrentViewMode();
				locked = tabInternal.bLocked;
				addressLocked = tabInternal.bAddressLocked;
			}

			std::wstring toString()
			{
				return _T("location = ") + location
					+ _T(", viewMode = ") + std::to_wstring(viewMode)
					+ _T(", locked = ") + std::to_wstring(locked)
					+ _T(", addressLocked = ") + std::to_wstring(addressLocked);
			}
		};

		TabsApi(TabContainerInterface *tabContainer);
		~TabsApi();

		std::vector<Tab> getAll();
		boost::optional<Tab> get(int tabId);
		void create(std::wstring path);
		int move(int tabId, int newIndex);
		bool close(int tabId);

		/* Events. */
		int addTabCreatedObserver(sol::protected_function observer);
		void removeTabCreatedObserver(int id);

	private:

		void onTabCreated(int tabId, sol::protected_function observer);

		TabContainerInterface *m_tabContainer;

		int m_observerIdCounter;
		std::unordered_map<int, boost::signals2::connection> m_tabCreatedConnections;
	};
}