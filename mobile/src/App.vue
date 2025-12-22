<template>
  <main class="container">
    <CircleButton
        class="button"
        :label="isLocked ? 'Locked' : 'Unlocked'"
        :color="isLocked ? 'green' : 'red'"
        @click="isLocked = !isLocked"
    />
  </main>

  <footer>
    <div class="connection-status">Status: {{ isConnected ? 'Connected' : 'Not Connected' }}</div>
  </footer>
</template>

<script setup lang="ts">
import {ref} from "vue";

import CircleButton from "./component/CircleButton.vue";

const isConnected = ref<boolean>(false);
const isLocked = ref<boolean>(false);


</script>

<style scoped lang="scss">
.container {
  height: 100%;
  width: 100%;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;

  .button {
    width: 10em;
    height: 10em;
  }
}
</style>