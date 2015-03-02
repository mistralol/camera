using System;
using System.Web.Security;

namespace WebUI.Code
{
	public class CameraRoleProvider : RoleProvider
	{
		public CameraRoleProvider ()
		{
		}

		public override void Initialize (string name, System.Collections.Specialized.NameValueCollection config)
		{
			base.Initialize (name, config);
		}


		public override void AddUsersToRoles (string[] usernames, string[] rolenames)
		{
			throw new NotImplementedException ();
		}

		public override string ApplicationName {
			get {
				throw new NotImplementedException ();
			}
			set {
				throw new NotImplementedException ();
			}
		}

		public override void CreateRole (string rolename)
		{
			throw new NotImplementedException ();
		}

		public override bool DeleteRole (string rolename, bool throwOnPopulatedRole)
		{
			throw new NotImplementedException ();
		}

		public override string[] FindUsersInRole (string roleName, string usernameToMatch)
		{
			throw new NotImplementedException ();
		}

		public override string[] GetRolesForUser (string username)
		{
			throw new NotImplementedException ();
		}

		public override string[] GetUsersInRole (string rolename)
		{
			throw new NotImplementedException ();
		}

		public override bool IsUserInRole (string username, string rolename)
		{
			throw new NotImplementedException ();
		}

		public override void RemoveUsersFromRoles (string[] usernames, string[] rolenames)
		{
			throw new NotImplementedException ();
		}

		public override bool RoleExists (string rolename)
		{
			throw new NotImplementedException ();
		}

		public override string Description {
			get {
				return base.Description;
			}
		}

		public override string[] GetAllRoles ()
		{
			throw new NotImplementedException ();
		}

		public override string Name {
			get {
				return base.Name;
			}
		}
	}
}

