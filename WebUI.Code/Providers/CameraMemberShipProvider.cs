using System;
using System.Web.Security;

namespace WebUI.Code
{
	public class CameraMemberShipProvider : System.Web.Security.MembershipProvider
	{
		private CameraClient Camera;

		public CameraMemberShipProvider ()
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
				return "CameraMemberShipProvider";
			}
		}

		public override string ApplicationName {
			get {
				return "Camera";
			}
			set {

			}
		}

		public override string Description {
			get {
				return "Camera MemberShip Provider";
			}
		}

		public override bool ChangePassword (string name, string oldPwd, string newPwd)
		{
			int ret = Camera.UserAuth (name, oldPwd);
			if (ret < 0)
				throw(new Exception ());
			if (ret == 0)
				return false;
			return true;
		}

		public override bool ChangePasswordQuestionAndAnswer (string name, string password, string newPwdQuestion, string newPwdAnswer)
		{
			throw(new NotSupportedException ());
		}

		public override MembershipUser CreateUser (string username, string password, string email, string pwdQuestion, string pwdAnswer, bool isApproved, object providerUserKey, out MembershipCreateStatus status)
		{
			int ret = Camera.UserCreate (username, password);
			if (ret < 0)
				throw(new Exception ());
			status = MembershipCreateStatus.Success;
			MembershipUser tmp = new MembershipUser (this.Name, username, username, email, "", "", true, false, DateTime.UtcNow, DateTime.UtcNow, DateTime.UtcNow, DateTime.UtcNow, DateTime.MinValue);
			return tmp;
		}

		public override bool DeleteUser (string name, bool deleteAllRelatedData)
		{
			if (Camera.UserDelete (name) < 0)
				throw(new Exception ());
			return true;
		}

		public override bool EnablePasswordReset {
			get {
				return true;
			}
		}

		public override bool EnablePasswordRetrieval {
			get {
				return false;
			}
		}

		public override MembershipUserCollection FindUsersByEmail (string emailToMatch, int pageIndex, int pageSize, out int totalRecords)
		{
			//We dont use email
			MembershipUserCollection col = new MembershipUserCollection();
			totalRecords = 0;
			return col;
		}

		public override MembershipUserCollection FindUsersByName (string nameToMatch, int pageIndex, int pageSize, out int totalRecords)
		{
			throw new NotImplementedException ();
		}

		public override MembershipUserCollection GetAllUsers (int pageIndex, int pageSize, out int totalRecords)
		{
			MembershipUserCollection col = new MembershipUserCollection ();

			throw new NotImplementedException ();
			return col;
		}

		public override int GetNumberOfUsersOnline ()
		{
			return 0;
		}

		public override string GetPassword (string name, string answer)
		{
			throw(new NotSupportedException ());
		}

		public override MembershipUser GetUser (object providerUserKey, bool userIsOnline)
		{
			throw new NotImplementedException ();
		}

		public override MembershipUser GetUser (string name, bool userIsOnline)
		{
			throw new NotImplementedException ();
		}

		public override string GetUserNameByEmail (string email)
		{
			throw new NotImplementedException ();
		}

		public override int MaxInvalidPasswordAttempts {
			get {
				return 5;
			}
		}

		public override int MinRequiredNonAlphanumericCharacters {
			get {
				return 0;
			}
		}

		public override int MinRequiredPasswordLength {
			get {
				return 5;
			}
		}

		public override int PasswordAttemptWindow {
			get {
				return 5;
			}
		}

		public override MembershipPasswordFormat PasswordFormat {
			get {
				return MembershipPasswordFormat.Hashed;
			}
		}

		public override string PasswordStrengthRegularExpression {
			get {
				return "";
			}
		}

		public override bool RequiresQuestionAndAnswer {
			get {
				return false;
			}
		}

		public override bool RequiresUniqueEmail {
			get {
				return false;
			}
		}

		public override string ResetPassword (string name, string answer)
		{
			throw(new NotSupportedException ());
		}

		public override bool UnlockUser (string userName)
		{
			throw(new NotSupportedException ());
		}

		public override void UpdateUser (MembershipUser user)
		{
			throw(new NotSupportedException ());
		}

		public override bool ValidateUser (string name, string password)
		{
			int ret = Camera.UserAuth (name, password);
			if (ret < 0)
				throw(new Exception ());
			if (ret == 0)
				return false;
			return true;
		}
	}
}

