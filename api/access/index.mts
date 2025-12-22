import {
  KMSClient,
  SignCommand,
} from '@aws-sdk/client-kms';

interface LambdaEvent {
  requestContext: {
    identity: {
      'cognito:groups': string[],
    },
  },
}

export const handler = async (event: LambdaEvent) => {
  const client = new KMSClient();

  const keyName = process.env.SIGN_KEY_ALIAS;
  if (!keyName) {
    return {
      statusCode: 500,
      body: JSON.stringify({ error: 'No key name is configured for signing.' }),
    };
  }

  const identity = event.requestContext.identity;
  const isResident = identity["cognito:groups"].includes('Resident');
  if (!isResident) {
    return {
      statusCode: 403,
      body: JSON.stringify({ error: 'User is unauthorized to perform action.' }),
    };
  }

  const command = `authorized@${Date.now()}`;
  const { Signature: signature } = await client.send(new SignCommand({
    KeyId: `alias/${keyName}`,
    Message: Buffer.from(command, 'ascii'),
    SigningAlgorithm: 'ECDSA_SHA_256',
  }));

  if (!signature) {
    return {
      statusCode: 500,
      body: JSON.stringify({ error: 'Failed to sign.' }),
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
