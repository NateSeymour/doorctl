import {
  KMSClient,
  SignCommand,
} from '@aws-sdk/client-kms';

interface LambdaEvent {
  requestContext: {
    identity: {
      'cognito:groups'?: string[],
    },
  },
}

interface ResponseBody {
  error?: string,
  token?: string,
}

const response = (code: number, body: ResponseBody) => {
  return {
    statusCode: code,
    body: JSON.stringify(body),
    headers: {
      "Content-Type": "application/json",
      "Access-Control-Allow-Origin": "*",
    },
  };
};

export const handler = async (event: LambdaEvent) => {
  try {
    const client = new KMSClient();

    const keyName = process.env.SIGN_KEY_ALIAS;
    if (!keyName) {
      return response(500, { error: 'No key name configured for signing.' });
    }

    const identity = event.requestContext.identity;
    const isResident = identity["cognito:groups"]?.includes('Resident');
    if (!isResident) {
      return response(403, { error: 'User is unauthorized to perform action.' });
    }

    const command = `authorized@${Date.now()}`;
    const { Signature: signature } = await client.send(new SignCommand({
      KeyId: `alias/${keyName}`,
      Message: Buffer.from(command, 'ascii'),
      SigningAlgorithm: 'ECDSA_SHA_256',
    }));

    if (!signature) {
      return response(500, { error: 'Failed to get signature from KMS.' });
    }

    return response(200, { token: `${command}@${Buffer.from(signature).toString('base64')}` });
  } catch (_) {
    return response(500, { error: 'Encountered unknown error.' });
  }
};
