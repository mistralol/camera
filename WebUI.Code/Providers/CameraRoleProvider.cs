using System;
using System.Collections.Generic;
using System.Web.Security;

namespace WebUI.Code.Providers
{
	public class CameraRoleProvider : RoleProvider
	{
		private CameraClient Camera;

		public CameraRoleProvider ()
		{
		}

		public override void Initialize (string name, System.Collections.Specialized.NameValueCollection config)
		{
			Camera = new CameraClient();
			Camera.Connect("unix:/tmp/CameraServer");
			base.Initialize (name, config);
		}

		public override string Name {
			get {
				return "CameraRoleProvider";
			}
		}

		public override string ApplicationName {
			get {
				return "Camera";
			}
			set {
				//not used
			}
		}

		public override string Description {
			get {
				return "Camera Role Provider";
			}
		}

		public override void AddUsersToRoles (string[] usernames, string[] rolenames)
		{
			foreach(string user in usernames)
			{
				foreach(string role in rolenames)
				{
					Camera.GroupUserAdd(role, user);
				}
			}
		}

		public override void CreateRole (string rolename)
		{
			if (Camera.GroupCreate (rolename) < 0)
				throw(new Exception ());
		}

		public override bool DeleteRole (string rolename, bool throwOnPopulatedRole)
		{
			if (throwOnPopulatedRole) {
				throw(new NotImplementedException ());
			}

			if (Camera.GroupDelete (rolename) < 0)
				throw(new Exception ());
			return true;
		}

		public override string[] FindUsersInRole (string roleName, string usernameToMatch)
		{
			List<string> result = new List<string>();
			StringVector lst = new StringVector ();
			try {
				if (Camera.GroupListUsers (roleName, lst) < 0)
					throw(new Exception ());

				for (int i =0; i<lst.Count; i++) {
					if (lst [i].Contains (usernameToMatch))
						result.Add (lst [i]);
				}
		
			} catch(Exception ex){
				throw(ex);
			} finally {
				lst.Dispose ();
			}
		
			return result.ToArray ();
		}

		public override string[] GetRolesForUser (string username)
		{
			List<string> result = new List<string> ();
			StringVector lst = new StringVector ();
			try {
				if (Camera.GroupList (lst) < 0)
					throw(new Exception());

				for(int i =0;i<lst.Count;i++)
				{
					int value = 0;
					if (Camera.GroupIsUserInGroup(lst[i], username, out value) < 0)
						throw(new Exception());
					if (value == 1)
						result.Add(lst[i]);
				}
			} catch(Exception ex) {
					throw(ex);
			} finally {
				lst.Dispose ();
			}
			return result.ToArray();
		}

		public override string[] GetUsersInRole (string rolename)
		{
			List<string> result = new List<string> ();
			StringVector lst = new StringVector ();
			try {
				if (Camera.GroupListUsers(rolename, lst) < 0)
					throw(new Exception());

				for(int i =0;i<lst.Count;i++)
					result.Add(lst[i]);

			} catch(Exception ex){
				throw(ex);
			} finally{
				lst.Dispose ();
			}
			return result.ToArray ();
		}

		public override bool IsUserInRole (string username, string rolename)
		{
			int value = 0;
			if (Camera.GroupIsUserInGroup(rolename, username, out value) < 0)
				throw(new Exception());
			if (value == 1)
				return true;
			return false;
		}

		public override void RemoveUsersFromRoles (string[] usernames, string[] rolenames)
		{
			foreach (string user in usernames)
			{
				foreach (string group in rolenames) {
					if (Camera.GroupUserRemove (user, group) < 0)
						throw(new Exception ());
				}
			}
		}

		public override bool RoleExists (string rolename)
		{
			int ret = Camera.GroupExists (rolename);
			if (ret < 0)
				throw(new Exception ());
			if (ret == 0)
				return false;
			return true;
		}

		public override string[] GetAllRoles ()
		{
			List<string> results = new List<string> ();
			StringVector lst = new StringVector ();
			try {
				if (Camera.GroupList(lst) < 0)
					throw(new Exception());
				for(int i =0;i<lst.Count;i++)
					results.Add(lst[i]);
			} catch(Exception ex){
				throw(ex);
			} finally {
				lst.Dispose ();
			}
			return results.ToArray ();
		}


	}
}

