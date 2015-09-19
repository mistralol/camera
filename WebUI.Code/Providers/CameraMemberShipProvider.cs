using System;
using System.Web.Security;

namespace WebUI.Code.Providers
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
            int ret = Camera.UserSetPassword(name, newPwd);
			if (ret < 0)
				throw(new CameraClientException(ret));
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
            if (Camera.UserExists(username) > 0)
                throw(new MembershipCreateUserException(MembershipCreateStatus.DuplicateUserName));
			int ret = Camera.UserCreate (username, password, email);
			if (ret < 0)
				throw(new CameraClientException(ret));
            ret = Camera.UserSetApproved(username, true);
            if (ret < 0)
                throw (new CameraClientException(ret));
			status = MembershipCreateStatus.Success;
            return GetUser(username, false);
		}

		public override bool DeleteUser (string name, bool deleteAllRelatedData)
		{
            int ret = Camera.UserDelete(name);
			if (ret < 0)
				throw(new CameraClientException(ret));
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

            StringVector lst = new StringVector();
            int ret = Camera.UserList(lst);
            if (ret < 0)
                throw new CameraClientException(ret);

            for (int i = 0; i < lst.Count; i++)
            {
                string user = lst[i];
                MembershipUser mu = GetUser(user, false);
                col.Add(mu);
            }
            totalRecords = lst.Count;
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
            throw (new NotSupportedException());
		}

		public override MembershipUser GetUser (string name, bool userIsOnline)
		{
            int ret = Camera.UserExists(name);
            if (ret < 0)
                throw (new CameraClientException(ret));
            UserItem info = new UserItem();
            ret = Camera.UserInfo(name, info);
            if (ret < 0)
                throw (new CameraClientException(ret));

            DateTime epoch = new DateTime(1970, 1, 1, 0, 0, 0, 0).ToLocalTime();
            MembershipUser mu = new MembershipUser(Name, info.Username, info.Key, info.EMail, string.Empty, string.Empty, info.IsApproved, info.IsLockedOut,
                Util.FromUnixTimeStamp(info.Created), Util.FromUnixTimeStamp(info.LastLoginDate), Util.FromUnixTimeStamp(info.LastActivityDate),
                Util.FromUnixTimeStamp(info.LastPasswordChange), Util.FromUnixTimeStamp(info.LastLockoutDate));
            info.Dispose();
            return mu;
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
            int ret = Camera.UserSetLockedOut(userName, false);
            if (ret < 0)
                throw (new CameraClientException(ret));
            return true;
		}

		public override void UpdateUser (MembershipUser user)
		{
            int ret = Camera.UserSetApproved(user.UserName, user.IsApproved);
            if (ret < 0)
                throw (new CameraClientException(ret));
            ret = Camera.UserSetLockedOut(user.UserName, user.IsLockedOut);
            if (ret < 0)
                throw (new CameraClientException(ret));
            if (user.IsOnline)
            {
                Camera.UserTouch(user.UserName);
            }
		}

		public override bool ValidateUser (string name, string password)
		{
			int ret = Camera.UserAuth (name, password);
			if (ret < 0)
				throw(new CameraClientException(ret));
			if (ret == 0)
				return false;
			return true;
		}
	}
}

