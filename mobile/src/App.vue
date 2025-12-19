<template>
  <main class="container">
    <CircleButton
        class="button"
        :label="unlocked ? 'Unlocked' : 'Locked'"
        :color="unlocked ? 'red' : 'green'"
        @click="unlocked = !unlocked"
    />
  </main>

  <footer>
    <div class="connection-status">Status: {{ connected ? 'Connected' : 'Not Connected' }}</div>
  </footer>
</template>

<script setup lang="ts">
import {ref} from "vue";

import CircleButton from "./component/CircleButton.vue";

const connected = ref<boolean>(false);
const unlocked = ref<boolean>(false);
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