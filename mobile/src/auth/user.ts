import { UserManager } from "oidc-client-ts";

export const userManager = new UserManager({
  authority: process.env.AUTH_AUTHORITY!,
  client_id: process.env.AUTH_AUTHORITY!,
  redirect_uri: "doorctl://auth",
  response_type: "code",
  scope: "email openid"
});
