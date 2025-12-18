import {
  KMSClient,
  SignCommand,
} from '@aws-sdk/client-kms';

export const handler = async () => {
  const client = new KMSClient();

  const keyName = process.env.SIGN_KEY_ALIAS;
  if (!keyName) {
    return {
      statusCode: 500,
      message: 'No key name is configured for signing.',
    };
  }

  const command = `unlock@${Date.now()}`;
  const { Signature: signature } = await client.send(new SignCommand({
    KeyId: `alias/${keyName}`,
    Message: Buffer.from(command, 'ascii'),
    SigningAlgorithm: 'ECDSA_SHA_256',
  }));

  if (!signature) {
    return {
      statusCode: 500,
      message: 'Failed to sign command.',
    };
  }

  return {
    statusCode: 200,
    body: JSON.stringify({
      command,
      signature: Buffer.from(signature).toString('base64'),
    }),
  };
};
