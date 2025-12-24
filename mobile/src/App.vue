<template>
  <header>
    <div v-if="isLoggedIn">Email: {{ identity.email }}</div>
  </header>
  <main class="container">
    <div v-if="isReady" class="locking-mechanism">
      <CircleButton
          class="button"
          :label="isLocked ? 'Locked' : 'Unlocked'"
          :color="isLocked ? 'green' : 'red'"
          @click="isLocked = !isLocked"
      />
    </div>
    <div v-else class="loading">
      <div v-for="_ in 3" class="dot"></div>
    </div>
  </main>
  <footer>
    <div>Connection Status: {{ isConnected ? "Connected" : "Not Connected" }}</div>
  </footer>
</template>

<script setup lang="ts">
import {computed, onMounted, ref} from "vue";
import {openUrl} from "@tauri-apps/plugin-opener";
import {buildLoginUrl} from "./util/auth.ts";
import {getCurrent} from "@tauri-apps/plugin-deep-link";
import {LazyStore} from "@tauri-apps/plugin-store";

import CircleButton from "./component/CircleButton.vue";

const token = ref<string | null>(null);
const identity = computed(() => {
  if (!token.value) return null;

  const [_, payload] = token.value.split(".");
  const identity = JSON.parse(atob(payload));
  console.log(identity);
  if (identity.exp > Date.now() / 1000) {
    return identity;
  }

  return null;
});

const isLoggedIn = computed(() => !!identity.value);
const isConnected = ref<boolean>(false);

const isReady = computed(() => {
  return isLoggedIn.value && isConnected.value;
});

const isLocked = ref<boolean>(false);

onMounted(async () => {
  const store = new LazyStore("auth.json");
  token.value = await store.get<string>("token") || null;

  if (isLoggedIn.value) return;

  // Check if callback from deeplink
  const startUrls = await getCurrent();
  if (startUrls) {
    for (const raw of startUrls) {
      const url = new URL(raw);

      const code = url.searchParams.get("code");
      if (code) {
        const response = await fetch(`${import.meta.env.VITE_AUTH_AUTHORITY}/oauth2/token`, {
          method: "POST",
          body: new URLSearchParams({
            "grant_type": "authorization_code",
            "code": code,
            "client_id": import.meta.env.VITE_AUTH_CLIENT_ID,
            "redirect_uri": "doorctl://auth",
          }),
        });

        if (response.ok) {
          const payload = await response.json();

          await store.set("token", payload.id_token);
          token.value = payload.id_token;
        } else {
          console.error(response);

        }
      }
    }
  }

  if (!isLoggedIn.value) {
    const loginUrl = buildLoginUrl({
      authority: import.meta.env.VITE_AUTH_AUTHORITY,
      clientId: import.meta.env.VITE_AUTH_CLIENT_ID,
      redirectUri: 'doorctl://auth',
      responseType: 'code',
      scope: ['email', 'openid'],
    });
    await openUrl(loginUrl);
  }
});
</script>

<style scoped lang="scss">
header {
  padding: 1em;
}

.container {
  width: 100%;
  display: grid;
  place-items: center;

  .button {
    width: 70vw;
    height: 70vw;
  }

  .loading {
    width: 70vw;
    height: 70vw;
    border-radius: 100%;
    border: 2px solid white;
    display: flex;
    flex-direction: row;
    align-items: center;
    justify-content: center;
    gap: 1em;

    .dot {
      width: 2em;
      height: 2em;
      background: white;
      border-radius: 100%;
      animation: dot-loading-anim 500ms ease-in-out alternate infinite;
    }

    @for $i from 1 through 3 {
      :nth-child(#{$i}n) {
        animation-delay: #{$i * 100}ms;
      }
    }
    
    @keyframes dot-loading-anim {
      from {
        transform: scaleY(0.5);
      }

      to {
        transform: scaleY(1.5);
      }
    }
  }
}

footer {
  padding: 1em;
}
</style>